[English](./README.md) | [简体中文](./README-zh.md) | 日本語

# 概要

Milk-V 
DuoはCV1800Bをベースにした超小型の組み込みプラットフォームです。LinuxとRTOSが実行可能で、プロフェッショナル、産業用ODM、AIoT、DIY、クリエイターに、高信頼で高費用対効果なプラットフォームを提供します。

## ハードウェア
### Milk-V Duo 64M
- CPU: CVITEK CV1800B (C906@1GHz + C906@700MHz)
- デュアルコアRV64 最大1GHz
- 64MB RAM
- アドオンボード経由での10/100Mbpsイーサネット

### Milk-V Duo 256M
- CPU: SOPHGO SG2002 (C906@1GHz + C906@700MHz + ARM Cortex-A53@1GHz)
- 256MB RAM
- GPIO経由でのオーディオ出力
- 1TOPSのNPU

### Milk-V Duo S
- CPU: SOPHGO SG2000 (C906@1GHz + C906@700MHz + ARM Cortex-A53@1GHz)
- 256MB RAM
- GPIO経由でのオーディオ出力
- 2系統のMIPI CSI
# SDKのディレクトリ構造

```
├── build               // コンパイルスクリプトとボード設定
├── build.sh            // 自動コンパイルスクリプト
├── buildroot-2021.05   // Buildrootのソースコード
├── freertos            // FreeRTOSのシステム
├── fsbl                // 完成済みfsblファームウェア
├── install             // 一時イメージの仮置き場
├── isp_tuning          // カメラ効果パラメータ
├── linux_5.10          // Linuxカーネル
├── middleware          // 自家製マルチメディアフレームワーク
├── device              // Milk-Vのコンフィグレーションファイル
├── opensbi             // opensbiライブラリ
├── out                 // 完成したSDカード用イメージはここに出てきます
├── ramdisk             // 完成済みramsidk
└── u-boot-2021.10      // u-bootのソースコード
```

# クイックスタート

> [ヒント]
> 
このSDKのコンパイル方法や使用法については、[公式ドキュメント](https://milkv.io/docs/duo/getting-started/buildroot-sdk)も参照してください。また、私達の公式ドキュメントのサイトはオープンソースです。もし内容の充実や多言語への翻訳に興味があるなら、ぜひ[このリポジトリ](https://github.com/milk-v/milkv.io/)にPRを飛ばしてください。

コンパイル環境を準備する前に、ローカルのubuntuを使用する際にサポートされているのは`「Ubuntu Jammy 22.04.x amd64」`のみです。

他のLinuxディストロを使用している場合、問題を回避するためにDocker環境を使用することを強くお勧めします。

2つの環境でのコンパイル方法について解説します。

## 1. Ubuntu 22.04を使用してコンパイル

### インストールするパッケージ

コンパイルに必要なパッケージをインストールします。

```bash
sudo apt install -y pkg-config build-essential ninja-build automake autoconf libtool wget curl git gcc libssl-dev bc slib squashfs-tools android-sdk-libsparse-utils jq python3-distutils scons 
parallel tree python3-dev python3-pip device-tree-compiler ssh cpio fakeroot libncurses5 flex bison libncurses5-dev genext2fs rsync unzip dosfstools mtools tcl openssh-client cmake expect
```

### SDKのソースコードを入手

```bash
git clone https://github.com/milkv-duo/duo-buildroot-sdk.git --depth=1
```

### <1>. 自動コンパイル

自動コンパイルスクリプト`build.sh`を実行:
```bash
cd duo-buildroot-sdk/
./build.sh
```
コンパイルスクリプトの用法を見ることができます。
```bash
# ./build.sh
Usage:
./build.sh              - Show this menu
./build.sh lunch        - Select a board to build
./build.sh [board]      - Build [board] directly, supported boards asfollows:
milkv-duo
milkv-duo256m
```
下に表示されるのは現在サポートされているターゲットです。

プロンプトにあるように、ターゲット向けにコンパイルするには2つ方法があります。

1つ目の方法は、`build.sh lunch`でインタラクティブメニューを立ち上げて、ターゲットのバージョンの番号を選択し、Enterを押すだけです。
```bash
# ./build.sh lunch
Select a target to build:
1. milkv-duo
2. milkv-duo256m
Which would you like:
```

２つ目の方法はスクリプトの引数に直にターゲットのバージョンを渡す方法です。例えば、`milkv-duo`向けにコンパイルするにはこうします。
```bash
# ./build.sh milkv-duo
```

正常にコンパイルされるとSDカード用イメージ`milkv-duo-*-*.img`が`out`ディレクトリの中に出てきます。

*注意：最初のコンパイル時に必要なツールチェーン(およそ840MB)が自動でダウンロードされます。一度ダウンロードされると`host-tools`内に自動で展開されます。以後のコンパイルでは`host-tools`ディレクトリがある場合再びダウンロードはされません。*

### <2>. 手動コンパイル

もし自動コンパイルスクリプトを実行していない場合、ツールチェーンを手動でダウンロードする必要があります。
[host-tools](https://sophon-file.sophon.cn/sophon-prod-s3/drive/23/03/07/16/host-tools.tar.gz)をダウンロードして、SDKのディレクトリに展開してください。
```bash
tar -xf host-tools.tar.gz -C /your/sdk/path/
```

手動コンパイルするには以下のコマンドを順を追って実行してください。コマンド中の`[board]`と`[config]`はターゲットとするハードウェアに置き換えてください。
現在サポートされている`[board]`と`[config]`は以下のとおりです。
```
milkv-duo               cv1800b_milkv_duo_sd
milkv-duo256m           cv1812cp_milkv_duo256m_sd
```

```bash
source device/[board]/boardconfig.sh

source build/milkvsetup.sh
defconfig [config]
clean_all
build_all
pack_sd_image
```

たとえば、`milkv-duo`向けにイメージをコンパイルするコマンドは以下のとおりですs。
```bash
source device/milkv-duo/boardconfig.sh

source build/milkvsetup.sh
defconfig cv1800b_milkv_duo_sd
clean_all
build_all
pack_sd_image
```

生成されたファームウェアの場所:
```
Duo:      install/soc_cv1800b_milkv_duo_sd/[board].img
Duo256M:  install/soc_cv1812cp_milkv_duo256m_sd/[board].img
```

## 2. Dockerを使用してコンパイル

DockerはホストのLinuxシステムからサポートされている必要があります。Dockerの使い方については[公式ドキュメント](https://docs.docker.com/)とか他の使い方を見てください。

SDKのソースコードをLinuxホストシステムに置いて、Milk-Vから提供されているコンパイル用イメージをDockerで呼び出してコンパイルします。

### SDKのソースコードをホストに持ってくる

```bash
git clone https://github.com/milkv-duo/duo-buildroot-sdk.git --depth=1
```

### SDKのコードディレクトリに入る

```bash
cd duo-buildroot-sdk
```

### Dockerイメージを持ってきて実行する

```bash
docker run -itd --name duodocker -v $(pwd):/home/work milkvtech/milkv-duo:latest /bin/bash
```

コマンド中のパラメータについて:
- `duodocker` Dockerの名前です。好きな名前を使えます。
- `$(pwd)` カレントディレクトリです。ここでは先程cdしたduo-buildroot-sdkを指しています。
- `-v $(pwd):/home/work` 現在のコードディレクトリをDockerイメージの/home/workに結びつけます。
- `milkvtech/milkv-duo:latest` Milk-VによるDockerイメージです。最初にhub.docker.comから自動でダウンロードされます。

Dockerが正常に実行されたら`docker ps -a`コマンドで実行状態を見ることができます。
```bash
$ docker ps -a
CONTAINER ID   IMAGE                        COMMAND       CREATED       STATUS       PORTS     NAMES
8edea33c2239   milkvtech/milkv-duo:latest   "/bin/bash"   2 hours ago   Up 2 hours             duodocker
```

### <1>. Dockerを使用して自動コンパイル

```bash
docker exec -it duodocker /bin/bash -c "cd /home/work && cat /etc/issue && ./build.sh [board]"
```
`./build.sh [board]`のコマンドの終端についてはUbuntu 20.04での自動コンパイルスクリプトでの使い方と同じです。`./build.sh`で使い方が表示され、`./build.sh lunch`で選択画面を表示できます。`./build.sh [board]`で直接ターゲット向けにコンパイルできます。`[board]`の部分は以下に置き換えられます。ターゲットに合わせてください。

```
milkv-duo
milkv-duo256m
```

コマンド中のパラメータについて:
- `duodocker` 実行中のDockerの名前です。先程設定したものと同じである必要があります。
- `"*"` クオートの中にDockerイメージ中で実行したいコマンドが入ります。
- `cd /home/work` /home/workディレクトリに移動します。このディレクトリは実行時にホストのコードディレクトリに紐付けられているため、Docker中の/home/workディレクトリはSDKのコードディレクトリになります。
- `cat /etc/issue` Dockerで実行されているイメージのバージョンを表示します。これはいまのところ「Ubuntu 22.04.3 LTS」で、デバッグに使われます。
- `./build.sh [board]` 自動コンパイルスクリプトを実行します。

たとえば、`milkv-duo`向けにコンパイルする場合には以下のようにします。
```bash
docker exec -it duodocker /bin/bash -c "cd /home/work && cat /etc/issue && ./build.sh milkv-duo"
```

コンパイルが成功すると、`out`ディレクトリの中に`milkv-duo-*-*.img`が出てきます。

### <2>. Dockerを使用して手動コンパイル

もし自動コンパイルスクリプトを実行していない場合、手動でツールチェイン[host-tools](https://sophon-file.sophon.cn/sophon-prod-s3/drive/23/03/07/16/host-tools.tar.gz)をダウンロードする必要があります。そしてそれをSDKのディレクトリに展開してください。
```bash
tar -xf host-tools.tar.gz -C /your/sdk/path/
```

手動コンパイルをする場合はDockerにログインする必要があります。`docker ps -a`コマンドを使用して8edea33c2239みたいな形式のコンテナのIDを表示します。

Dockerに入る:
```bash
docker exec -it 8edea33c2239 /bin/bash
```

Dockerに紐付けられたコードディレクトリに入る：
```bash
root@8edea33c2239:/# cd /home/work/
```

そして、手動コンパイルを実行するためのコマンドを実行していきます。コマンド中の`[board]`と`[config]`はコンパイルするターゲットに合わせてください。
今のところサポートされている`board`と対応する`config`は以下のとおりです:
```
milkv-duo               cv1800b_milkv_duo_sd
milkv-duo256m           cv1812cp_milkv_duo256m_sd
```

```bash
source device/[board]/boardconfig.sh

source build/milkvsetup.sh
defconfig [config]
clean_all
build_all
pack_sd_image
```

たとえば、`milkv-duo`向けにイメージをコンパイルする場合のコンパイル手順は以下のとおりです。
```bash
source device/milkv-duo/boardconfig.sh

source build/milkvsetup.sh
defconfig cv1800b_milkv_duo_sd
clean_all
build_all
pack_sd_image
```

生成されたファームウェアの場所:
```
Duo:      install/soc_cv1800b_milkv_duo_sd/[board].img
Duo256M:  install/soc_cv1812cp_milkv_duo256m_sd/[board].img
```

コンパイルが完了したら`exit`コマンドでDockerから抜けれます:
```bash
root@8edea33c2239:/home/work# exit
```
生成されたイメージはホストのコードディレクトリからも見れます。

### Dockerを停止する

コンパイルが完了して、もし上のDocker環境がもう必要ないなら止めて削除できます:
```bash
docker stop 8edea33c2239
docker rm 8edea33c2239
```

## 3. その他の環境でのコンパイルに関する注意

もしこのSDKを上の2つの環境以外で行いたいなら、参考までに以下のことに注意してください。

### Cmakeのバージョン

注意：`cmake`は最低でもバージョン`3.16.5`以降が必要です。

システムの`cmake`バージョンの確認

```bash
cmake --version
```

例として、`Ubuntu 20.04`のaptでインストールされる`cmake`のバージョンは

```
cmake version 3.16.3
```
です。条件を満たしていないので手動で最新の`cmake`をインストールしてください

```bash
wget https://github.com/Kitware/CMake/releases/download/v3.27.6/cmake-3.27.6-linux-x86_64.sh
chmod +x cmake-3.27.6-linux-x86_64.sh
sudo sh cmake-3.27.6-linux-x86_64.sh --skip-license --prefix=/usr/local/
```

手動でインストールした`cmake`は`/usr/local/bin`に配置されます。`cmake --version`すると以下のように出力されるはずです。

```
cmake version 3.27.6
```

### Windows Subsystem for Linux(WSL)を使用してコンパイル

WSLでコンパイルするには少し問題があります。
Windowsとの互換性のために環境変数$PATHにいくつかのスペース文字を含むWindows用の環境変数が入っていて、一部でパスが通りません。

解決するには`/etc/wsl.conf`ファイルに以下を追記します。

```
[interop]
appendWindowsPath = false
```

その後、`wsl.exe --reboot`でWSLを再起動する必要があります。そうすれば手動コンパイルを行うことができます。
変更を元に戻すには、`/etc/wsl.conf`の`appendWindowsPath`をtrueに設定します。WSLを再起動すれば、また$PATHからWindowsの環境変数が使えるようになります。

## SDカードへの書き込み

> 注意：書き込むとmicroSDカード内のデータはすべて削除されます。重要なデータはバックアップしてください！！！
- Windows上で生成したイメージを書き込む場合`balenaEtcher`や`Rufus`、`Win32 Disk Imager`などのツールを使えます。
- Linux上で生成したイメージを書き込む場合ddコマンドを使えます。 **ddコマンドで書き込む場合指定するデバイスが書き込むmicroSDカードであることを十二分に確認してください**

  ```bash
  sudo dd if=milkv-duo-*-*.img of=/dev/sdX
  ```

## 電源投入

- Milk-V DuoのmicroSDカードスロットにmicroSDカードを挿入します。
- シリアルケーブルを接続します。(必ずしも必要ではありません)
- 電源に接続するとシステムが立ち上がります。
- シリアルケーブルが接続されている場合、ブートログをシリアルコンソールから見ることができます(`mobarXterm`、`Xshell` 、`Tera Term`など)。システムが立ち上がればコンソールからログインしてLinuxコマンドを実行できます。

### Duoのターミナルに入る方法

- シリアルケーブルを使う
- USBネットワークを使う(CDC-NCM)
- イーサネットインターフェースを使う(アドオンボードが必要)

Duoのターミナルに入る際に必要なユーザー名とパスワードは以下の通りです。

```
root
milkv
```

### LEDの点滅の無効化

もしDuoに載っているLEDの点滅を無効にしたいならDuoのターミナルで以下のコマンドを実行してください。

   ```bash
   mv /mnt/system/blink.sh /mnt/system/blink.sh_backup && sync
   ```

以上のコマンドはLEDの点滅スクリプトをリネームしています。そしてDuoを再起動するとLEDは点滅しなくなります。

DuoのLEDをまた点滅させたい場合、スクリプトのファイル名を元に戻します。

   ```bash
   mv /mnt/system/blink.sh_backup /mnt/system/blink.sh && sync
   ```

### IO-Boardを使う

IO-Boardを使用する場合、USBネットワーク(CDC-NCM)は使用できないので、IO-Boardのイーサネットインターフェースを使用してください。
IO-BoardのEthernetポートに固定MACアドレスを割り当てる必要がある場合は、以下のコマンドを実行してください(**コマンド中のMACアドレスは設定したいMACアドレスに置き換えてください。また、同一ネットワークセグメント内の異なるデバイスのMACアドレスは重複してはいけません**)。

   ```bash
   echo "pre-up ifconfig eth0 hw ether 78:01:B3:FC:E8:55" >> /etc/network/interfaces"
   ```
   - それからボードを再起動してください。

IO-Board上の4発のUSBポートを有効化する：

   ```bash
   ln -sf /mnt/system/usb-host.sh /mnt/system/usb.sh
   sync
   ```
   - それからボードを再起動してください。

たとえば、USBフラッシュドライブがIO-Board上のUSBポートに接続されている場合`ls /dev/sd*`で認識していることを確認できます。

USBフラッシュドライブをマウントしてその内容を見る(/dev/sda1を例に)。

   ```bash
   mkdir /mnt/udisk
   mount /dev/sda1 /mnt/udisk
   ```

`/mnt/udisk`ディレクトリの内容が予想と一致するか確認する。

   ```bash
   ls /mnt/udisk
   ```

USBフラッシュドライブをアンマウントするコマンド。

   ```bash
   umount /mnt/udisk
   ```

USBネットワーク(CDC-NCM)の機能をIO-Board不使用時に使う。

   ```bash
   ln -sf /mnt/system/usb-ncm.sh /mnt/system/usb.sh
   sync
   ```
   - それからボードを再起動してください。

# よくある質問

1. なぜ1つのコアしか表示されないのですか。

   CV1800Bチップはデュアルコアですが、現在、Linuxシステムは1つのコアで実行され、もう1つのコアはリアルタイムシステムの実行に使用されています。このコアの使用については[公式ドキュメント](https://milkv.io/docs/duo/getting-started/rtoscore)をお読みください。

2. なぜ28MBしかRAMが使えないのですか。
 
RAMの一部が[ION](https://github.com/milkv-duo/duo-buildroot-sdk/blob/develop/build/boards/default/dts/cv180x/cv180x_default_memmap.dtsi#L15)(カメラを使ってアルゴリズムを実行するときに使用するメモリ)に割り当てられているからです。カメラを使用しない場合は、この[ION_SIZE](https://github.com/milkv-duo/duo-buildroot-sdk/blob/develop/build/boards/cv180x/cv1800b_milkv_duo_sd/memmap.py#L43)の値を0に変更し、再コンパイルしてください(Duo 
256M: [ION_SIZE](https://github.com/milkv-duo/duo-buildroot-sdk/blob/develop/build/boards/cv181x/cv1812cp_milkv_duo256m_sd/memmap.py#L43))。

## チップ製造元のドキュメント

- CV181x/CV180x MMF SDK開発ドキュメント (英語と中国語): [MultiMedia Framework Software Development Document](https://developer.sophgo.com/thread/471.html)
- CVシリーズのTPU SDK開発ドキュメント (中国語): [CV series chip TPU SDK development data summary](https://developer.sophgo.com/thread/473.html)

# Milk-Vについて

- [公式ウェブサイト](https://milkv.io/)

# フォーラム

- [MilkV Community](https://community.milkv.io/)

