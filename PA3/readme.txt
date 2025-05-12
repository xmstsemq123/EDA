1. 學號：b11107055
2. 姓名：呂佳祐
3. 使用之程式語言：C++
4. 使用之編譯平台：Linux GNU g++
5. 檔案壓縮方式: tar zcvf b11107055-p3.tgz b11107055-p3
6. 各檔案說明：
	b11107055-p1/main.cpp		  	: 主程式source code
	b11107055-p1/AStarAlgorithm.h  	: A*演算法程式的模板
	b11107055-p1/AStarAlgorithm.cpp : A*演算法程式
	b11107055-p1/ReadFile.h  	  	: 讀取輸入檔的程式模板
	b11107055-p1/ReadFile.cpp  	  	: 讀取輸入檔的程式
	b11107055-p1/Output.h  	      	: 輸出.out檔的程式模板
	b11107055-p1/Output.cpp  	  	: 輸出.out檔的程式
	b11107055-p1/report.docx  	  	: word報告
  	b11107055-p1/Makefile		  	: Makefile，用來告訴編譯器如何編譯
	b11107055-p1/readme.txt		  	: 本檔案
7. 編譯方式說明：        	
   主程式：
	   在 b11107055-p3/ 這個資料夾下指令 : make
	   即可在 b11107055-p3 產生 picRouting 執行檔
	
8. 執行、使用方式說明：
	主程式：
   	    compile 完成後，在 b11107055-p3/ 目錄下會產生一個 picRouting 執行檔
   		執行檔的命令格式為 :
   		./picRouting [input file name] [output file name]
		ex: ./picRouting pic5x5.in pic5x5.out
		輸入命令後，每對pin會自動繞線，繞線結果會儲存至輸出檔案裡。

		*以下例外情況會提示錯誤並終止picRouting程式：
		*1. 執行檔命令參數有多或有缺
		*2. 無法讀取或開啟input file
		*3. 無法寫入或開啟output file