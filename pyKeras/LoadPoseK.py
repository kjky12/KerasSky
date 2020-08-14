# -*- coding: utf-8 -*-
"""
Created on Thu Dec 03 22:51:57 2015

@author: admin
"""
import os
import numpy as np
from array import array
import Readini as rdini
import struct
#포즈 개수


def load_data( ):
	X=[]
	X1=[] # train
	X2=[] # test

        path = os.getcwd() + "\\Lerning_Lab_csv\\" # Curent path
        dirCsv = os.listdir(path)                 # How many directorys are there
        listCnt = []
        for dirTmp in dirCsv:
            i = 0
            fileName = os.listdir(path + dirTmp)
            for fileTmp in fileName :                    
                    #start to open file
                    if(fileTmp.find(".csv") > -1) :
                            filepath = path + dirTmp + "\\" + fileTmp
                            f= open ("%s" % filepath, 'r')
                            j=0
                            for line in f:
                                    fields = line.split(',')
                                    fields.append(fileTmp[0:len(fileTmp) - 4])            #Distinguish pose
                                    listCnt += fileTmp[0:len(fileTmp) - 4]
                                    sample = np.array(fields)
                                    X.append(sample)
                                    j+=1
                                    #if  j > 1300 : break                                        
                            print  "Finsh to lerning : " + dirTmp + "\t=> " + fileTmp
                            f.close()
                            i = i + 1
                    else:
                            FieldCnt = int(rdini.ReadConfig('config.ini', 'POSE_FIELD_CNT'))     #ini config 파일을 읽어서 값을 넣는다.                            
                            #filepath = path + dirTmp + "\\" + fileTmp
                            #size = os.path.getsize(filepath)
                            #f= open ("%s" % filepath, 'rb')
                            #binaryData = f.read(size)
                            #unpackCnt = "%df" % (size / 4)
                            #TupleData = struct.unpack(unpackCnt, binaryData)
                            #for i in range(0, len(TupleData), FieldCnt ) :
                            #        fields = list(TupleData[i : i + FieldCnt])
                            #        for j in range(0, len(fields)):
                            #                fields[j] = str(fields[j])
                            #                fields[len(fields) - 1] += '\n'                                            
                            #        fields.append(fileTmp[0:len(fileTmp) - 4])            #Distinguish pose
                            #        sample = np.array(fields)
                            #        #X.append(sample)
                            #print  "Finsh to lerning : " + dirTmp + "\t=> " + fileTmp
                            #f.close()
                            #i = i + 1
                            
                                        
            print "%s - Cnt : %d \n" % (dirTmp, i)
            listCnt = list(set(listCnt))
            FileCount = len(listCnt)
            
                      
	X=np.array(X)				
	np.random.shuffle(X)

	print X.shape, len(listCnt)
		#  X_train , y_train , X_test, y_test
	return  (X[:40000, :-1], X[:40000, -1]), (X[40000:, :-1], X[40000:, -1]), FileCount
	

        #원본
	return  (X[:40000, :-1], X[:40000, -1]), (X[40000:, :-1], X[40000:, -1]), FileCount
        




# 기존 데이터파일 삭제(테스트 O) - 전체 초기화 df_MESS_PY_LEARNING_ALL_INIT
def All_Init_Data( ):
        path = os.getcwd() + "\\Lerning_Lab_csv\\" # Curent path
        dirCsv = os.listdir(path)
        for dirTmp in dirCsv:
                fileName = os.listdir(path + dirTmp)
                for fileTmp in fileName :
                        filepath = path + dirTmp + "\\" + fileTmp
                        #print (filepath)
                        os.remove(filepath)     #파일 삭제


# 기존 데이터파일 삭제(테스트 O) - 부분 초기화 df_MESS_PY_LEARNING_POSE_INIT
def Pose_Init_Data(PossNumber):   
        path = os.getcwd() + "\\Lerning_Lab_csv\\" # Curent path
        dirCsv = os.listdir(path)
        for dirTmp in dirCsv:
                fileName = os.listdir(path + dirTmp)
                for fileTmp in fileName :
                        if(PossNumber == int( fileTmp[0:len(fileTmp) - 4] ) ) :
                                filepath = path + dirTmp + "\\" + fileTmp
                                os.remove(filepath)     #파일 삭제
                                        

# 현재 몇개의 자세가 있는지 확인 (테스트 O)
def Count_Pose():   
        path = os.getcwd() + "\\Lerning_Lab_csv\\" # Curent path
        dirCsv = os.listdir(path)                 # How many directorys are there
        FileCount = 0
        listCnt = []
        for dirTmp in dirCsv:
            i = 0
            fileName = os.listdir(path + dirTmp)
            for fileTmp in fileName :
                    listCnt += fileTmp[0:len(fileTmp) - 4]

            listCnt = list(set(listCnt))
            FileCount = len(listCnt)
        
        return FileCount


#리스트가 들어왔을때 csv파일로 변경
def Add_Data_base(nPose, listValue):
        try :
                print(listValue)
                path = os.getcwd() + "\\Lerning_Lab_csv\\base"
                strFileName = "%d.csv"% nPose                   #포즈번호
                path = path + "\\" + strFileName
                
                strTT = "" # 초기화
                FieldCnt = int(rdini.ReadConfig('config.ini', 'POSE_FIELD_CNT'))     #ini config 파일을 읽어서 값을 넣는다.
                for i in range(0, len(listValue), FieldCnt ) :
                        csvField = str(",".join(map(str, listValue[i : i + FieldCnt])))
                        csvField += "\r\n"        # 63(ini에 작성된 config임)개 이후 다음 줄로
                        strTT += csvField
                
                f = open(path, 'ab')
                f.write(strTT)
                f.close()
                return "Success to add data"
        except Exception:
                return "Error!!"



#들어왔을때 그냥 dat파일로 저장
def Add_Data_base_binary(nPose, binaryValue):
        try :
                path = os.getcwd() + "\\Lerning_Lab_csv\\base"
                strFileName = "%d.dat"% nPose                   #포즈번호
                path = path + "\\" + strFileName
                
                f = open(path, 'ab')
                f.write(binaryValue)
                f.close()
                return "Success to add data(binary)"
        except Exception:
                return "Error!!"
        

                
if __name__ == "__main__" :
	load_data()        

