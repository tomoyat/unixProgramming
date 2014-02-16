# ファイルとディレクトリ

## stat関数
stat関数は3種類ありそれぞれファイルに関する情報を返す

- stat : pathnameで指定したファイルの情報を返す
- fstat : ファイルディスクリプタで指定した情報を返す
- lstat : pathnameで指定したファイルがシンボリックリンクの場合そちらを返す

## ファイルの種類

- 普通のファイル
- ディレクトリファイル : 書き込めるのはカーネルのみ
- 文字型特殊ファイル
- ブロック型特殊ファイル
- FIFO : プロセス間通信に使用されるファイル。名前付きパイプ
- ソケット : プロセス間のネットワーク通信に使用されるファイル
- シンボリックリンク

## セットユーザID,セットユーザグループID

- 実ユーザID,実グループID : 実際に誰であるかをしめす。ログイン時にパスワードファイルから取得
- 実効ユーザID,実効グループID,補足グループ : ファイルアクセスのパーミッション用（補足グループとはユーザが複数のグループに所属した場合）
- 保存セットユーザID,グループID : プログラムを実行した際の実効ユーザ,グループIDのコピー

プログラムを実行した際に、実行したユーザに関わらず、そのプログラムのプロセスの実効ユーザとして、
ファイルの所有者を設定することができる機能がある。

## ファイルアクセスパーミッション

- どんなファイルをオープンするときにも、そのパスのすべてのディレクトリの実行許可が必要
- ディレクトリに書き込みと実行許可がないとそのディレクトリに新しいファイルを作成できない
- 既存ファイルを削除するには、そのファイル収めたディレクトリの書き込み許可と実行許可が必要
  - ファイル自体の読み取りや書き込み許可の必要はない
- exec関数のいずれかを用いてファイルを実行するには、ファイルが普通のファイルであり、実行許可が必要

### プロセスがファイルをオープンしたりするときのパーミッション

プロセスがファイルをオープンしたり、作成したり、削除するときのカーネルが行う
パーミッションのチェックは以下の通り

- 前提として、カーネルが行うパーミッションのチェックはファイルの所有者とプロセスの実効ユーザID,グループID,補足グループIDに依存する

1. プロセスの実効ユーザIDが0(root)の場合アクセスは許可される
2. プロセスの実効ユーザIDがファイルの所有者IDと等しい場合
  - 所有者のパーミッションが正しければOK、それ以外は許可しない
3. プロセスの実効グループIDまたは補足グループIDの一つがファイルのグループIDと等しい場合
  - グループアクセスのパーミッションが設定されていればOK,それ以外は許可しない
4. 他ユーザのアクセスのパーミッションが設定されていればOK,それ以外は許可しない

## 新規ファイルやディレクトリの所有権

- 新規ファイルのユーザIDはプロセスの実効ユーザID
- 新規ファイルのグループIDは以下のいずれか
  1. プロセスの実効グループID
  2. そのファイルを収めるディレクトリのグループID

## access関数

access関数を使用するとセットユーザIDが指定されていても、
実ユーザでパーミッションをチェックできる

```c
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
  if (argc != 2) exit(1);
  // access関数でチェック
  if (access(argv[1], R_OK) < 0) printf("access error\n");
  else printf("read access OK\n");
  // fileをオープン
  if (open(argv[1], O_RDONLY) < 0) printf("open error\n");
  else printf("open for read\n");

  return 0;
}
```

上のプログラムを試してみる
```bash
# vagrantユーザのhomeにhogeがある
[vagrant@localhost ~]$ ls -l
total 0
-rw-r----- 1 vagrant vagrant 0 Feb 16 14:27 hoge
# 別ユーザで上記のプログラムを実行してopenできるか調べる
$  ls -l
-rwxrwxr-x 1 tomoyat tomoyat 6952  2月 16 14:33 2014 a.out
$ ./a.out /home/vagrant/hoge
access error
open error
# openできない
# そこで
# vagrantユーザにa.outの所有権を変更して、セットユーザIDのフラグをたてる
[vagrant@localhost ~]$ sudo chown vagrant /home/tomoya/workspace/accessFuncTest/a.out
[vagrant@localhost ~]$ sudo chmod u+s /home/tomoya/workspace/accessFuncTest/a.out
# もう一度実行
$ ls -l
-rwsrwxr-x 1 vagrant tomoyat 6952  2月 16 14:33 2014 a.out
$ ./a.out /home/vagrant/hoge
access error
open for read
# access関数はエラーを返すがopenはできる
```

## umask関数

umask関数でパーミッションのフラグを立てると、今後クリエイトされるファイルは、
umaskで立てられたパーミッションの条件を指定できなくなる

## スティッキービット

これが設定されていると、プロセスが終了したあともプログラムのテキストセグメントをスワップ空間に保持する。
linuxではこのような実装はない

## ファイルの切り詰め

trancateやftrancate関数を使うとファイルを切り詰められる。

## ファイルシステム

- ディスクのパーティションの中にiリストと呼ばれるものがある
- iリストの中にはiノードが入っている
- iノードは固定長の項目でファイルに関するほとんどの情報が入っている
- iノードにはそのファイルのデータがどこのデータブロックにあるかを覚えている
- 各iノードには、そのiノードを指すディレクトリの個数をカウントするリンクカウントがある
- リンクカウントが0になって初めてファイルが削除できる
- シンボリックリンクの場合、iノードのファイルの種類がS_IFLNKになる
- ディレクトリブロックにはそのディレクトリにあるファイルのiノードの情報が入っている

## link,unlink

任意のファイルは複数のディレクトリに所属することができる。ただしできるのはスーパユーザだけ。なぜならファイルシステムにループを作る危険性があるから。（link関数を使用する）

unlink関数は指定されたpathのディレクトリ項目を削除し、リンクカウントを減少させる。
ファイルに対して別のリンクがあればそこから参照できる。
リンクカウントが0になるとファイルは削除できる。プロセスがファイルをオープンしていると、ファイルは削除されない。
ファイルがクローズされるとカーネルはまずオープンしてるプロセスの個数を検査し、これが0であると次にリンクカウントを検査し0であるとファイルの内容を削除する。

プログラムがクラッシュした場合でも、作成した一時ファイルが残らないようにするために、unlinkの性質を使う。
open等でファイルを作成したあと直ちにunlinkを呼ぶ。ファイルはopenされているので削除はされないが、closeされり、プロセスが終了したり（カーネルがファイルをクローズ）した場合にファイルは削除される。

## ファイルの時刻

各ファイルにおいて3つの時刻フィールドが管理されている。

- 最終参照時刻 : readとか
- 最終修正時刻　：writeとか
- 最終状態変更時刻 : こちらはiノードの変更をしめす。chmod、chown等

## chdir,fchdir,getcwd

プロセスにはカレント作業ディレクトリがあり、このディレクトリは相対パスを探す起点となる。
カレント作業ディレクトリはプロセスの属性の一つであり、変更する場合はchdirかfchdir関数を使用する。
getcwd関数は現在のカレント作業ディレクトリの絶対パスを取得する。

## sync,fsync

ファイルにデータを書くと（write）と、データはカーネルのバッファの一つにコピーされる。
syncが呼ばれるとバッファキャッシュをディスクに書き込む
