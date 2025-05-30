# Electronic Design Automation (EDA) Programming Assignments

本儲存庫編錄《電子設計自動化》課程中完成的三份程式作業，標題如下：

* HW1: 實體設計可視觀化
* PA2: 單高度 cell 合法化 Legalization
* PA3: 光子組合電路 (PIC) 詳細繞線

所有作業均使用 C++ 程式語言编寫，並可於 Linux 環境下編譯與執行。

---

## 🔹 HW1 — 實體設計可視觀化 (Gnuplot)

### 作業目的

將 DEF 格式的配置資料，轉換為 Gnuplot 可視化圖形輸出檔，方便用戶根據 layout 檢視 placement 結果。

### 執行流程

1. 讀取 input.txt 內容，包含 die area、components、metal layers
2. 判讀後用 fstream 輸出 .gp 檔
3. 使用 Gnuplot 開啟該 .gp 檔以顯示 layout

### 執行方式

```bash
make
./genPlot input.txt output.gp
```

### 程式組成

* `main.cpp`：主程式
* `genPlot.cpp / .h`：編譯與輸出 Gnuplot 格式資料
* `Makefile`：一鍵編譯配置
* `readme.txt`：作業操作說明

---

## 🔹 PA2 — Single-Row Cell Legalization

### 作業目的

將配置結果的 cell 重新合法化 placement，使其對齊 row 和 site，無重疊、方向符合預設，並保持 displacement 最小化。

### 輸入格式 DEF (簡化)

```text
VERSION 5.8 ;
DIEAREA ( x0 y0 ) ( x1 y1 ) ;
ROW core_SITE_ROW_0 core 0 0 N DO 50 BY 1 STEP 200 0 ;
COMPONENTS 8 ;
- cellName + PLACED ( x y ) N ;
...
END COMPONENTS
END DESIGN
```

### 輸出格式

格式與輸入 DEF 相同，僅變更 placement 位置。

### 執行方式

```bash
make
./legalizer [cell_width] [alpha] [input.def] [output.def]
# 範例: ./legalizer 500 0.5 case1.def LG_case1.def
```

### Cost Function

```math
total_cost = avg_displacement + α × max_displacement
```

### 程式組成

* `main.cpp`：主執行流程
* `Algorithm2.*`：排序 cell 功能
* `ReadDEF.*`：讀取 DEF 檔案
* `Output.*`：輸出補偵結果
* `Displacement.*`：計算 displacement cost
* `Makefile` 與 `readme.txt`：編譯說明

### 錯誤處理

* 參數錯誤
* 無法開啟 input/output 檔
* cell 數量超過 die 範圍則可能進入無窮回歸

---

## 🔹 PA3 — Photonic IC Detailed Routing

### 作業目的

為 Photonic Integrated Circuits 進行詳細繞線，將所有 2-pin 網路用 waveguide 連接，同時最小化繞線總損耗（waveguide loss）。

### 繞線 Loss 模型

* **Propagation loss**: 線長 × α
* **Crossing loss**: grid 上有複數 nets 交會時額外增加
* **Bending loss**: 每次轉角增加損耗

### 輸入格式

```text
grid 5 5
propagation loss 1
crossing loss 10
bending loss 3
num net 3
0 0 1 4 1
1 3 3 0 3
2 4 4 1 1
```

### 輸出格式

```text
<net_id> <number of routes>
x1 y1 x2 y2
x2 y2 x3 y3
...
```

* 必須為繩直線，不可有斜線
* 路線距點前後順序順續

### 執行方式

```bash
make
./picRouting [input_file.in] [output_file.out]
# 範例: ./picRouting pic5x5.in pic5x5.out
```

### 程式組成

* `main.cpp`：主執行機制
* `AStarAlgorithm.*`：執行 A\* 繞線實作
* `ReadFile.*`：輸入解析
* `Output.*`：輸出結果
* `Makefile` / `readme.txt`：操作與說明文件

### 錯誤處理

* 缺少參數
* 無法開啟/ 讀取輸入檔

---

## 🛠️ 開發環境

* **語言**: C++
* **編譯**: GNU g++ on Linux
* **編譯工具**: Makefile
* **輔助軟體**: Gnuplot (only in HW1)

---

📃 *本專案為課程作業之結果，僅供學術用途，請勿使於商業或不當使用。*
