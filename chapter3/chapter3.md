# ファイル入出力
open,read,write,lseek,closeだけでほぼすべてのファイル入出力を行える。
この章ででてくる関数は、アンバッファド入出力。

## ファイル記述子

- カーネルはopenしたファイルをすべてファイル記述子（ディスクリプタ）で参照する。
- ファイルディスクリプタは非負の整数
- readやwriteするさいはファイルディスクリプタを指定してファイルを識別する

## lseek

- openされた個々のファイルにはカレントファイルオフセットが関連づけられている
- カレントファイルオフセットは非負の整数で、ファイルの先頭から数えたバイト数
- 読み書きは、カレントファイルオフセットから開始され、読み書きしたバイト数だけオフセットが増加する

## ファイルの共有

- 各プロセスごとにプロセステーブルが存在する
  - プロセステーブルにはそのプロセスがオープンしたファイルディスクリプタのテーブルがある
  - ファイルディスクリプタのフラグとファイルディスクリプタのテーブルへのポインタをセットで持つ
- オープンされたファイルのためのテーブル。カーネルが管理する
  - テーブルには以下の項目が含まれる
	- 読み書き、追記、ブロック塔のフィアルに対する状態のフラグ
	- カレントファイルオフセット
	- ファイルに対するvノードテーブルへのポインタ
- オープンされた各ファイルに対して、vノード構造体がある
  - vノードにはファイルに関する情報が入っている
  - vノードにはiノードも含まれる

異なるのプロセスが同じファイルをオープンした場合、プロセステーブル、ファイルテーブルは、
それぞれのプロセスが独立して持つが、vノードテーブルは同一のモノをさす。
プロセスがforkした場合ファイルテーブルも共有される。

そのため、ファイルに追記しようとした際、lseek->writeという手順をそれぞれ異なるプロセスが
実行した場合、片方の変更をもう片方のプロセスが上書きしてしまう可能性がある。

そうならないように、アトミック操作（複数の手順がすべて実行されるか、すべて実行されないか）
で行う必要がある。

## dupとdup2

- dupやdup2関数を使うとファイルディスクリプタを複製できる
- fd2 = dup(fd)とやると、fd,fd2ともに同一のファイルテーブルをさす

## fcntl
fcntl関数はオープンしているファイルの属性を変更できる。
dupのようにファイルディスクリプタの複製も可能。

- プロセステーブルにはそれぞれのファイル記述子ごとにファイル記述子フラグがあり、close-on-execが定義されている。
- close-on-execとはexecが実行された際にファイルディスクリプタを閉じるかどうかを決定できる
- ファイルテーブルにはファイル状態フラグがあり、read onlyとかnon blockとかを指定できる

上記の設定もfcntlで変更できる。

## ioctl
上記でできないようなことは、すべてioctlで行う。デバイスドライバとかにも使われているらしい。