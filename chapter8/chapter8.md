#プロセス制御

##fork関数

- 親プロセスを複製して、新しいプロセスを作成する。
- 複製なので、ヒープやスタックなどのメモリ上のデータも複製される。
- 親も子もforkの呼び出しに続く命令から実行が継続される
- forkのあとにはexecが実行されることが多いので今時のforkはメモリを完全には複製しない
  - その代わりにcopy-on-write(COW)と呼ばれる技法が使われる
  - COWは読み取り領域を親と子を共有して、その領域が変更されたときにメモリにその一部分だけコピーする


##vfork関数

- vforkはforkと同じでプロセスを複製する
- ただし、領域のコピーは発生せず子がexecやexitを呼ぶまで親の空間で動作する
- vforkは子がexecやexitを呼ぶまで子が先に動作することを保証する

##exit関数

プロセスの終了方法は5通り

- mainで`return`を呼ぶ
- `exit`を呼ぶ。atexitを呼んで脱出ハンドラすべてを呼び出す。標準ファイルディスクリプタをcloseする。
- `_exit`か`_Exit`を呼ぶ。これは脱出ハンドラやシグナルハンドラは呼ばない。入出力をflushするかは実装依存
- プロセスの最後のスレッドの開始ルーティンで`return`を呼ぶ
- プロセスの最後のスレッドで`pthread_exit`を呼ぶ

異常終了は次の3つ

- `abort`を呼ぶ。シグナル`SIGABRT`を生起する
- プロセスが特定のシグナルを受け取った場合。例えばゼロ除算しようとしたときなど
- キャンセル要請に対する最後のスレッドの応答

プロセスがどうやって終了するかに関わらす、カーネル内以下のようなことが実行される

- プロセスがオプーンしたディスクリプタをすべてクローズする。
- プロセスが使用したメモリの解放
- etc

`exit status`と`termination status`は区別されていて、`exit`,`return`や`_exit`の引数は`exit status`で最終的にカーネルがそれを、`termination status`に変換している。
異常終了の場合は`termination status`をカーネルが生成する。

###子と親の終了するタイミングの関係

- 子の前に親が終了する場合、initプロセスが親が終了した子プロセスの親となる

- 親の前に子が終了する場合、子が完全に消えると子の状態を親がチェックできなくなるので、必要な情報を残した状態で子は生き続けている。
- この情報は親が`wait`や`waitpid`を呼んだときに入手できる
- 子が終了して、親が`wait`や`waitpid`を呼んでないと子プロセスはzombieとなって生き続ける

```sh
$ ps -fl
  UID   PID  PPID   C STIME   TTY           TIME CMD            F PRI NI       SZ    RSS WCHAN     S             ADDR
  501  5037  5036   0  5:40PM ttys001    0:00.00 (a.out)     2006   0  0        0      0 -      Z+                  0
```

##wait関数

プロセスが終了するとカーネルはその親に`SIGCHLD`を送る。また`wait`や`waitpid`を呼ぶと次のいずれかの状態になる

- ブロックする
- 子が終了していれば、終了状態を取得する
- 子プロセスがいない場合はエラーで戻る
- `waitpid`はブロックを防げる

`wait(&status)`を呼ぶと子の終了ステータスが`status`に入る。これをマクロを使うことでとりだすことができる。

```c
void pr_exit(int status) {
  if (WIFEXITED(status)) {
    printf("nomal termination, exit status = %d\n", WEXITSTATUS(status));
  } else if (WIFSIGNALED(status)) {
    printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status), WCOREDUMP(status) ? " (core file generated" : "");
  } else if (WIFSTOPPED(status)) {
    printf("child stopped, signal number = %d\n", WSTOPSIG(status));
  }
}
```

###ゾンビのさけ方

プロセスが子を`fork`するときに子の完了は待ってプロセスをブロックしたくなくて、もし子が先に終わったとしても、その子プロセスをzombieにしたくないときは、`fork`を2回呼ぶ方法がある

##レースコンディション

レースコンディション（競合状態）が発生するのは、複数のプロセスが共有データに対して何らかの操作を行い、その最終結果がプロセスの実行順序に依存するとき。例えば、同じログファイルに2つのプロセスがアンバッファドな書き込みを行うと、出力される文字の順序がぐちゃぐちゃになる

##exec関数郡
