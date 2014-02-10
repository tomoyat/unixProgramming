# UNIXの標準化と実装
UNIXの歴史とかとか

## 制限事項

- CのINT_MAXとかはlimits.hに定義されている
- だいたい制限事項はlimits.hに定義されている
- 実行時に判定する制限はsysconf,pathconf,fpathconfという関数で調べられる
