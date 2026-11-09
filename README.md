# LiNEA40A — せおと配列と右手トラックボールのカスタム設定

[keyfreaksさんのLiNEA40用ZMK設定](https://github.com/keyfreaks/zmk-config-LiNEA40)をベースに、せおと配列での日本語入力、右手だけでのマウス・ブラウザー操作、Windowsでの利用に合わせて調整した個人用ファームウェア設定です。[LiNEA40本体のリポジトリ](https://github.com/keyfreaks/LiNEA40)

> [!NOTE]
> **2026年11月9日時点の記録です。** キーマップや設定は日常的に書き換えているため、READMEや画像に載っている内容が現在の `main` と異なる場合があります。最新の割り当てや設定値は [`main` の設定ファイル](https://github.com/gilldish/zmk-config-LiNEA40/tree/main/config) を参照してください。

<a name="overview"></a>

## 変更点の概要

- [**キーマップ図**](#keymaps) — BASE、せおと裏面、マウス、数字、無線などの配置を画像で掲載。
- [**せおと配列と日本語入力**](#seoto) — BASEをせおと配列に変更。Space長押しで裏面を開き、「ん」「っ」、記号などを入力。
- [**オートマウスの解除**](#automouse) — 左クリックを離すとすぐ文字入力へ戻り、その後は最大200 msだけ連続クリックに対応。
- [**トラックボールとスクロール**](#pointing) — 一時的な低速化、ボールでのスクロール、ポインターの時計回り10°補正。
- [**Bluetoothとスリープ対策**](#bluetooth) — 接続間隔7.5 msを要求。専用ビルドでは、切断後のBLE再接続をキー押下まで待機。
- [**ショートカットとWindows連携**](#shortcuts) — キーの兼用、同時押し、Win＋矢印、YouTube操作などを追加。
- [**ビルドと参照ファイル**](#build) — GitHub Actionsの通常版とパッチ適用版、参照したリビジョン。

<a name="keymaps"></a>

## キーマップ図

2026年11月9日版 v06。画像をクリックすると個別に拡大できます。オレンジはコンボ、赤枠はそのレイヤーに入るキー、薄い三角は下位レイヤーへ透過するキーです。予約枠のL3はキー割り当てがすべて透過のため、図は省いています。

[![L0 BASEのキーマップ](docs/keymap/l00-base.png)](docs/keymap/l00-base.png)

[![L5 せおと裏面のキーマップ](docs/keymap/l05-seoto-ura.png)](docs/keymap/l05-seoto-ura.png)

<details>
<summary>残りのキーマップ図を表示</summary>

[![オートマウスの解除対象から外したキー](docs/keymap/aml-exclusions.png)](docs/keymap/aml-exclusions.png)

[![L1 ファンクションと矢印](docs/keymap/l01-fx-arrow.png)](docs/keymap/l01-fx-arrow.png)

[![L2 YouTube](docs/keymap/l02-youtube.png)](docs/keymap/l02-youtube.png)

[![L4 マウス](docs/keymap/l04-mouse.png)](docs/keymap/l04-mouse.png)

[![L6 記号](docs/keymap/l06-mark.png)](docs/keymap/l06-mark.png)

[![L7 数字とスクロール](docs/keymap/l07-number-scroll.png)](docs/keymap/l07-number-scroll.png)

[![L8 トラックボールスクロール](docs/keymap/l08-trackball-scroll.png)](docs/keymap/l08-trackball-scroll.png)

[![L9 トラックボール低速](docs/keymap/l09-mouse-slow.png)](docs/keymap/l09-mouse-slow.png)

[![L10 ブラウザー](docs/keymap/l10-browser.png)](docs/keymap/l10-browser.png)

[![L11 無線とメンテナンス](docs/keymap/l11-wireless.png)](docs/keymap/l11-wireless.png)

[![L12 連続左クリック](docs/keymap/l12-click-repeat.png)](docs/keymap/l12-click-repeat.png)

</details>

[ページ間リンク付きのキーマップPDF](docs/keymap/LiNEA40_keymap_2026-11-09_v06.pdf) · [概要に戻る](#overview)

<details>
<summary>レイヤーの入口と役割</summary>

| 層 | 名前 | 入口・役割 |
| --- | --- | --- |
| 0 | `BASE` | 通常入力。せおと配列と各レイヤーへの入口 |
| 1 | `fx_arrow` | Enter長押し。F1〜F12、矢印、Home／End、ページ移動 |
| 2 | `youtube` | 右下3キーの同時押し。動画・メディア操作 |
| 3 | `RESERVED_2` | 予約枠。キー割り当ては下位へ透過 |
| 4 | `MOUSE` | トラックボールを動かすと自動で有効。クリック、戻る／進む、スクロールへの入口 |
| 5 | `seoto_ura` | Space長押し。せおとの裏面 |
| 6 | `MARK` | Backspace長押し。記号と括弧、トラックボール低速、エンコーダーで音量 |
| 7 | `NUMBER_SCROLL` | Tab長押し。数字、Win＋矢印、トラックボールでスクロール |
| 8 | `trackball_scroll` | E長押し、またはL4の2段目右端のキーを保持。ボールでスクロール |
| 9 | `mouse_slow_1to` | L4の低速用キーを保持。一時的にポインターを低速化 |
| 10 | `browser` | W長押し。タブ切替、閉じる、閉じたタブを復元 |
| 11 | `WIRELESS` | Esc長押し。BLE・USBの切替、ペアリング、DFU、Studio解除、スリープ |
| 12 | `CLICK_REPEAT` | 左クリックを離した後、最大200 ms。左クリックの割り当てを一時的に残す |

</details>

<a name="seoto"></a>

## せおと配列と日本語入力

- **表面（L0）**は、作者版のQWERTYからせおと配列に置き換えています。WindowsではMozKeyの拡張ローマ字テーブルと組み合わせ、Q・J・X・Cなどをキーボードから送ってIME側で解釈します。[せおと配列の解説](https://note.com/firstyear_dev/n/ne4aedaad09b2) · [MozKey](https://github.com/koyasi777/mozkey)
- **裏面（L5）**は、Spaceを短押しすると空白、押しながら別のキーを打つと裏面になるSandS風の構成です。「ん」は `V → N`（IMEに対応するローマ字定義が必要）、「っ」は `L → T → U` を送ります。Backspace、ハイフン、`~ ! # @ ?`、Windowsの絵文字パネルもまとめています。
- **IME切替**は、A＋IでIME ONを送ってL0に戻り、N＋TでIME OFFを送ります。Windows側でIME ON／OFFに対応するキー設定が必要です。

<a name="automouse"></a>

## オートマウスの解除

作者版にもあるオートマウスを、文字入力と右手マウス操作を行き来しやすいように変更しています。ボール操作で **L4 MOUSE** に入り、入力が止まると **10秒** で解除されます。右側オーバーレイの `zip_temp_layer` がこの時間を管理します。

| 操作 | 動作 |
| --- | --- |
| 左クリックを押す・押し続ける | 左ボタンを押下したまま保持するため、そのままドラッグできる |
| 左クリックを離す | 左ボタンを解放してすぐL0へ戻る。同時にL12を有効にし、元の3か所の左クリックだけを最大200 ms残す |
| 有効時間内にもう一度左クリックする | 同じ場所で再クリックできる。再び離すと連続クリック用の時間を取り直す |
| L4の低速用キーを押す | `mouse_exit_mo9` がL4を解除し、押下中だけL9を有効にする。離すとL9とL4を解除 |
| L4でA＋Iを押す | IMEをONにし、L0へ戻る |

左クリックを離してからの **200 msは連続クリックの猶予**です。他の位置はL12から下位へ透過します。右・中クリックには左クリックと同じ解除マクロを割り当てていません。実装は `click_and_exit` と `sl_mouse` です。[左クリックで解除する実装の参考記事](https://qiita.com/kompeki/items/ead8f7d69e4bc00cb48c)

`excluded-positions` には14か所（`6 7 8 9 16 17 18 19 20 26 27 28 29 30`）を指定しています。これらの位置は、単押しでL4を解除する処理の対象外です。クリックやF20などのマウス側の割り当てを使うための設定で、左クリックのマクロによる解除は別に働きます。L4のV位置はマウスレイヤーのトグルです。[除外キーの図](docs/keymap/aml-exclusions.png) · [ZMKの一時レイヤー](https://zmk.dev/docs/keymaps/input-processors/temp-layer)

<a name="pointing"></a>

## トラックボールとスクロール

| 項目 | 11月9日時点の設定 |
| --- | --- |
| センサー | PMW3610、inorichi版ドライバー、ソフトウェアポーリング125 Hz |
| ポインター感度 | 通常700 CPI。L6・L9では400 CPIの低速モード |
| 角度補正 | パッチ適用版でポインターの相対移動を時計回りに10°回転。低速時の端数は次のレポートに繰り越す |
| ボールでスクロール | L7・L8でY軸を反転し、ボールの移動をホイール入力へ変換。縦ホイール倍率は1/5 |
| 左のロータリーエンコーダー | L0は縦スクロール、L1はその3倍、L6は音量、L7は上下矢印、L11は左右矢印 |
| ホイール基準値 | `ZMK_POINTING_DEFAULT_SCRL_VAL = 100` |

角度補正は、キーボードを斜めに置いたときの操作方向に合わせるための変更です。初期の20°から10°に調整しました。対象はトラックボール由来のポインター移動です。[角度補正を含むビルドのコミット](https://github.com/gilldish/zmk-config-LiNEA40/commit/6e3c9a8b9222fd9dfc6a34c336bbcac3556f8832)

<details>
<summary>入力タイミングとセンサーの補足</summary>

- `&mmv` は遅延20 ms、周期5 ms、最大速度まで100 ms、加速指数2。
- `&msc` は遅延3 ms、周期3 ms、最大速度まで100 ms、加速指数0。エンコーダーのホイール出力はタップ時間20 ms。
- 右側のセンサー設定にはRUNからの切替3,264 ms、REST1のサンプリング40 ms、REST1からの切替9,600 msがあります。これらは右側ファームウェアの参照値です。実際のL4の解除時間はオーバーレイの `10000` ms を参照してください。

</details>

<a name="bluetooth"></a>

## Bluetoothとスリープ対策

- **接続間隔の調整** — 右側の `CONFIG_BT_PERIPHERAL_PREF_MIN_INT` と `MAX_INT` をともに `6` に設定し、ホストに7.5 msを要求します。WindowsでトラックボールをBLE接続したときのレポート間隔のばらつきを減らすための調整です。実際の接続間隔はホストとの交渉で決まります。
- **キー押下まで再接続を待つパッチ** — ペアリング済みの使用中ホストと切断したとき、右側ファームウェアがホスト向けBLEアドバタイジングをいったん止めます。新しいキー押下、または明示的なBTプロファイル選択で再開します。PCの意図しないスリープ復帰を調べるために入れた変更で、対象は切断後の再接続です。バッテリー報告やトラックボールのポーリングは、それぞれ独立した設定です。
- **接続先と出力の切替** — L11にBT 0〜4を左右から選べる配置、BLE／USB出力切替、選択中のボンド消去と全消去、DFU、ZMK Studio解除を配置しています。
- **Windowsをスリープさせるキー** — L11の右側に `Win+X → U → S` を送るマクロを配置。キー押下で再接続するパッチとは別の機能です。

<a name="shortcuts"></a>

## ショートカットとWindows連携

| 変更 | 内容 |
| --- | --- |
| Layer-TapとMod-Tap | Layer-Tapは `hold-preferred`、Mod-Tapは `balanced`。L0のB・Vを短押しで文字、長押しで左右Shiftに割り当て |
| 修飾キーで割り当て変更 | PをAlt／Winと組み合わせるとTabを送り、Alt＋Tab／Win＋Tabを操作。左Ctrlと組み合わせるとK→貼り付け、M→切り取り、R→取り消し、S→全選択、N→保存、←→新規タブ、Z→タブを閉じる、F→再読み込み、G→検索 |
| ウィンドウ配置 | Tab長押しでL7を開く。方向キーを最初に押した時点でWinを保持し、同じ長押し中に方向キーを続けて入力。Tabを離すとWinを解放。Ctrl＋Win＋左右で仮想デスクトップも切替 |
| 同時押し | Escape、Insert、Delete、Ctrl＋Enter、IME切替、中クリック、最小化など。キー位置38＋39＋40は受付150 msで、L0・L4からL2へ切り替えてF19を送る |
| YouTubeとブラウザー | L2に再生・一時停止、シーク、全画面などを配置。L10ではタブ切替・閉じる・復元を右手で操作 |
| PC側のホットキー | F18はChatGPTの音声入力用。F19は別画面のYouTubeにフォーカス、F20はマウスポインターの画面切替用。F19／F20の画面操作にはWindows側のAutoHotkey設定を使用 |

<details>
<summary>コンボのキー位置と適用レイヤー</summary>

キー位置は図の小さな数字です。「共通」はレイヤー指定のないコンボです。

| 適用レイヤー | キー位置と動作 |
| --- | --- |
| 共通 | 11＋12でGrave、16＋17でIME ONとL0へ復帰、12＋13でIME OFF、9＋16でDelete、31＋32で最小化（Alt＋Space → N） |
| L0 | 6＋7でInsert、8＋9でDelete、28＋29でCtrl＋Enter |
| L0・L3 | 0＋1でEscape |
| L0・L4 | 38＋39＋40でL2へ切り替えてF19 |
| L1 | 8＋9でHome、18＋19でEnd、7＋16で `<`、7＋18で `>` |
| L2 | 28＋38で `<`、28＋40で `>`。38＋39＋40は入力を抑止 |
| L4 | 7＋8または17＋18で中クリック |

</details>

<a name="build"></a>

## ビルドと参照ファイル

ベースは **ZMK v0.2.1**。対象は右がトラックボール兼セントラル、左がロータリーエンコーダーのLiNEA40Aです。XIAO nRF52840向けに左右のUF2と `settings_reset` 用UF2をビルドします。

- **通常版** — `LiNEA40 original firmware MANUAL` を手動実行。リポジトリのキーマップと設定で、ZMKへの追加パッチを適用せずにビルドします。
- **パッチ適用版** — `★LiNEA40 keypress reconnect AUTO` を手動実行。11月9日に確認した成果物では、右側UF2にキー押下での再接続と角度補正のパッチを適用しています。成果物の `build-info/` でリビジョン、適用パッチ、ビルド設定、SHA-256を確認できます。[GitHub Actions](https://github.com/gilldish/zmk-config-LiNEA40/actions)

| 確認したい内容 | 参照先 |
| --- | --- |
| レイヤー、コンボ、マクロ、スクロール倍率 | [`config/LiNEA40.keymap`](config/LiNEA40.keymap) |
| AMLのタイムアウト・除外位置、低速／スクロールの対象レイヤー | [`LiNEA40_right.overlay`](config/boards/shields/LiNEA40/LiNEA40_right.overlay) |
| センサー設定・BLE接続間隔 | [`LiNEA40_right.conf`](config/boards/shields/LiNEA40/LiNEA40_right.conf) |
| ZMKと外部モジュールの版 | [`config/west.yml`](config/west.yml) |
| ビルド手順とZMKへの変更 | [`.github/workflows/`](.github/workflows/) · [`patches/`](patches/) |

キーマップ図 v06の元になった設定は [`3e2e996`](https://github.com/gilldish/zmk-config-LiNEA40/commit/3e2e996e707e219f6be1e7130d9ae7de3a23c309) です。角度補正の値と右側ファームウェアのビルド設定は、別の成果物 [`6e3c9a8`](https://github.com/gilldish/zmk-config-LiNEA40/commit/6e3c9a8b9222fd9dfc6a34c336bbcac3556f8832) で確認しています。

[概要に戻る](#overview)
