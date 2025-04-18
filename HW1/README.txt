1. 學號：b11107055
2. 姓名：呂佳祐
3. 使用之程式語言：C++
4. 使用之編譯平台：Linux GNU g++
5. 檔案壓縮方式: tar zcvf b11107055-p1.tgz b11107055-p1
6. 各檔案說明：
	b11107055-p1/main.cpp		: 主程式source code
	b11107055-p1/DEF2Gnu.h  	: header file，用來宣告轉換DEF檔至gp檔之模板
	b11107055-p1/DEF2Gnu.cpp 	: cpp file，用來定義轉換DEF檔至gp檔之轉換功能類別
  	b11107055-p1/Makefile		: Makefile，用來告訴編譯器如何編譯
	b11107055-p1/readme.txt		: 本檔案
7. 編譯方式說明：        	
   主程式：
	   在 b11107055-p1/ 這個資料夾下指令 : make
	   即可在 b11107055-p1 產生 genPlot 的執行檔
	
8. 執行、使用方式說明：
	主程式：
   	    compile 完成後，在 b11107055-p1/ 目錄下會產生一個 genPlot 的執行檔
   		執行檔的命令格式為 :
   		./genPlot [MSBCS width] [MSBCS height] [input file name] [output file name]
		ex: ./genPlot 7100 6600 CS8x8.def output.gp
		輸入命令後，會產生output.gp檔
		在linux環境下，輸入命令gnuplot output.gp，會產生output.png以及打開qt gui，圖片內容是在gnuplot繪圖軟體下DEF描述的電路圖

		*以下例外情況會提示錯誤並終止genPlot程式：
		*1. 執行檔命令參數有多或有缺
		*2. 無法開啟input file
		*3. DEF檔案內格式錯誤
