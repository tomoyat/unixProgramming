# chapter1
## ファイルとディレクトリ

- filenameに使えないのはスラッシュとnull
- 作業ディレクトリは相対パスの起点

## 入出力

- ファイル記述子（file descriptor）プロセスがファイルを識別するための非負の整数
- アンバッファド入出力（バッファリングしない）: open, read, write, lseek, close

## プログラムとプロセス

- 6つあるexec関数を呼び出すとカーネルはプログラムをメモリに読み込む
- プロセスを制御するためには、主にfork,exec,watipidの3つがある

## ANSI C

- unistd.hにはread,write,getpidとかがある
- _tで終わるデータ型を、基本的なシステムデータ型とよび、sys/tyepes.h(unistdが必要)で定義される

## エラー処理

- エラーが起きると多くの関数でエラー内容の情報を与えるerrno変数にエラーの番号を書き込む
- errnoはクリアされない
- strerror, perrorとか

## ユーザ識別

- ユーザIDが0のユーザをroot

## シグナル

- 特定の状況が発生したことをプロセスに通知する手法

## UNIXの時計

- カレンダー時間とはUTCの1970/01/01を起点とした経過秒数
- プロセス時間とはプロセスが使用したCPU資源をもとに測定
  - 実時間とはプロセスが実行に要した経過時間
  - ユーザCPU時間はユーザモードでプログラムを実行した時間
  - システムCPU時間はシステムモードでプログラムを実行した時間

## システムコールとライブラリ関数

- システムコールとはカーネルサービスを要求するサービスポイント

