# -*- coding: utf8 -*-

# socket 과 select 모듈 임포트
from socket import *
from select import *
import sys
from time import ctime
import os
import time
import struct

#딥러닝 추가
import LearningK
import LoadPoseK

#st_StandardMess
#통신 구조체 정의(메세지 총 사이즈 만큼 데이터화한다.)
# st_StandardMess[0] = int	nMessSize;			//! 메세지 사이즈
# st_StandardMess[1] = int	nMessID;			//! 메세지 ID
# st_StandardMess[2] = int	nTemp;			        //! Temp 변수



#학습 모드
df_MESS_PY_LEARNING_ALL_INIT            = 1001  #학습 모드 - 전체 초기화
df_MESS_PY_LEARNING_POSE_INIT           = 1002  #학습 모드 - 부분 초기화
df_MESS_PY_LEARNING_ADD_DATA            = 1003  #학습 모드 - 데이터 추가
df_MESS_PY_LEARNING_LEARN               = 1004  #학습 모드 - 학습


#인식 모드
df_MESS_PY_CHK_COUNT_POSE               = 2001  #인식 모드 - 자세 갯수 확인
df_MESS_PY_CHK_POSE                     = 2002  #인식 모드 - 현재 자세 확인


df_MESS_PY_RECV_ERROR   		=	1200	#! 서버 에러

PY_FAIL_DATA		                =	-9991	#Deep Lerning Fail




g_chkList = []
g_chkTuple = ()
g_chkStr = ''
g_chkint = 999

# 데이터의 유무 확인
def chkData(dataTmp):
    if dataTmp is not None: #데이터가 있으면 1 없으면 0 리턴
        return 1
    else :
        return 0

# 메세지만 전송.. - 클라이언트에게 메세지를 전송!
def SendMessage(socket_in_list, nMess):
    #메세지 전송완료!!
    nPacketSize = 8
    strStData = struct.pack('i',nPacketSize)
    strStData += struct.pack('i',nMess)
    strStData += struct.pack('i',0000)
    socket_in_list.send(strStData)


#튜플을 str에 바이트 맞춰 저장함
def SaveTupleToStruct(nMessID,listData):
    buff = ''
    if nMessID == df_MESS_PY_LEARNING_ALL_INIT	:    
        if chkData(tupleData[0]):                                        #float - 자세
            buff += struct.pack('f',tupleData[0])             
        else :                                                
            buff += struct.pack('f',PY_FAIL_DATA)

        if chkData(tupleData[1]):                                        #float - 신뢰도?
            buff += struct.pack('f',tupleData[1])             
        else :                                                
            buff += struct.pack('f',PY_FAIL_DATA)

        #if chkData(tupleData[2]):                                        #double		dCNMin;				//! 반송파 평균 잡음레벨 전력차		반송파 탐색 최소 C/N
        #    buff += struct.pack('d',tupleData[2])             
        #else :                                                
        #    buff += struct.pack('d',PY_QRY_NONE_DATA)
	#    
        return buff


# 호스트, 포트와 버퍼 사이즈를 지정
HOST = ''
PORT = 56789
RECV_BUFSIZE = 4096
BUFSIZE = 1024
ADDR = (HOST, PORT)

# 소켓 객체를 만들고..
serverSocket = socket(AF_INET, SOCK_STREAM)

# 서버 정보를 바인딩
serverSocket.bind(ADDR)

# 요청을 기다림(listen)
serverSocket.listen(10)
connection_list = [serverSocket]
print('==============================================')
print('Start server. %s Port waited to connection.' % str(PORT))
print('==============================================')

# 무한 루프를 시작
while connection_list:
    try:
        print('[INFO] Waited connection...')

        # select 로 요청을 받고, 10초마다 블럭킹을 해제하도록 함
        read_socket, write_socket, error_socket = select(connection_list, [], [], 10)

        for sock in read_socket:
            # 새로운 접속
            if sock == serverSocket:
                clientSocket, addr_info = serverSocket.accept()
                connection_list.append(clientSocket)
                print('[INFO][%s] Connect new clinet(%s).' % (ctime(), addr_info[0]))

                # 클라이언트로 응답을 돌려줌
                for socket_in_list in connection_list:
                    if socket_in_list != serverSocket and socket_in_list != sock:
                        try:
                            a=1
                        except Exception:
                            socket_in_list.close()
                            connection_list.remove(socket_in_list)
            # 접속한 사용자(클라이언트)로부터 새로운 데이터 받음
            else:
                data = sock.recv(RECV_BUFSIZE)
                if data:                    
                    nMessSize = struct.unpack('i', data[0:4])                    
                    #if(nMessSize[0] != len(data)): #보낸 데이터 사이즈와 받은 데이터 사이즈가 다르면 break(다음데이터를 기다린다)                        
                     #   print('[INFO][%s]Wait Next Data..' % ctime())
                      #  break
                    
                    for socket_in_list in connection_list:
                         if socket_in_list != serverSocket and socket_in_list == sock:
                             
                            try:
                                #st_StandardMess
                                #구조체 정의
                                # st_StandardMess[0] = int	nMessSize;			//! 메세지 사이즈
                                # st_StandardMess[1] = int	nMessID;			//! 메세지 ID
                                # st_StandardMess[2] = int	nTemp;			        //! Temp 변수
                                
                                nStep = 12
                                st_StandardMess = struct.unpack('iii', data[0:nStep])                                
                                
                                nMessSize   = st_StandardMess[0]
                                nMessID     = st_StandardMess[1]
                                nTemp       = st_StandardMess[2]
                                                              
                                print (str(nMessID))


                                
                                #f의 개수만큼... unpack를 해줘야함..(데이터를 보고 수정하는게 나을듯..)
                                #어느 변수에 넣을지도 확인도 같이해야할듯
                                #st_StandardMess = struct.unpack('f', data[0:nStep])
                                #이제 여기서..DeepLerning 판단을 함
                                #structlist이거에 튜플로 변형 시킴(메세지 전송을 위하여 헤더는 따로 추가하니 데이터만..)!!
                                structlist = []
                                if(nMessID == df_MESS_PY_LEARNING_ALL_INIT  ):            #학습 모드 - 전체 초기화(테스트 완료)
                                    print("LEARNING_ALL_INIT")
                                    LoadPoseK.All_Init_Data( )
                                elif(nMessID == df_MESS_PY_LEARNING_POSE_INIT ):          #학습 모드 - 부분 초기화(테스트 완료)
                                    print("LEARNING_POSE_INIT")
                                    pos = struct.unpack('i', data[nStep: nStep + 4])        #int형 한개
                                    LoadPoseK.Pose_Init_Data(pos[0])
                                elif(nMessID == df_MESS_PY_LEARNING_ADD_DATA  ):          #학습 모드 - 데이터 추가(테스트 완료)
                                    print("ADD_DATA")
                                    #학습 모드 데이터 추가 구조체
                                    #int	nAddPoseData		//! 추가할 자세 데이터 갯수
                                    #int	nPose			//! 추가할 자세
                                    #int	nPoseDataCnt		//! 자세 갯수(ex : 63)
                                    #float	fPoseData[]		//! 자세 데이터
                                    nAddPoseDataTmp = struct.unpack('3i', data[nStep:nStep + 12])                                    
                                    nStep += 12
                                    nAddPoseData    = nAddPoseDataTmp[0]#추가할 자세 개수
                                    nPose           = nAddPoseDataTmp[1] #자세
                                    nPoseDataCnt    = nAddPoseDataTmp[2]#자세 개수(ex : 63)
                                       
                                    unpackCnt = "%df" % int(nPoseDataCnt)
                                    nOneStep = (nPoseDataCnt * 4)
                                    for i in range(0, nAddPoseData):                                        
                                        fTuplePoseData = struct.unpack(unpackCnt, data[nStep:nStep + nOneStep])
                                        nStep += nOneStep

                                        print(LoadPoseK.Add_Data_base(nPose, list(fTuplePoseData)))

                                    
                                elif(nMessID == df_MESS_PY_LEARNING_LEARN     ):          #학습 모드 - 학습(테스트 완료)
                                    print("Lerning")
                                    LearningK.Lerning( )
                                elif(nMessID == df_MESS_PY_CHK_COUNT_POSE     ):          #인식 모드 - 자세 갯수 확인(테스트 완료)
                                    print("CHK_COUNT_POSE")
                                    FileCount =  LoadPoseK.Count_Pose()
                                    print(str(FileCount))

                                    structlist.append(FileCount)
                                elif(nMessID == df_MESS_PY_CHK_POSE           ):          #인식 모드 - 현재 자세 확인(테스트 완료)
                                    print("CHK_POSE")
                                    #인식 모드 - 자세 확인
                                    #int	nPoseDataCnt		//! 자세 갯수(ex : 63)                                    
                                    #float	fPoseData[]		//! 자세 데이터
                                    nAddPoseDataTmp = struct.unpack('i', data[nStep:nStep + 4])    #자세 갯수(ex : 63)
                                    nStep += 4
                                    
                                    nPoseDataCnt    = nAddPoseDataTmp[0]                            #확인할 자세 필드 개수
                                    
                                    unpackCnt = "%df" % int(nPoseDataCnt)
                                    nOneStep = (nPoseDataCnt * 4)
                                    fTuplePoseData = struct.unpack(unpackCnt, data[nStep:nStep + nOneStep])
                                    
                                    #인식 시도
                                    PosResult = LearningK.PoseCognition(fTuplePoseData)

                                    #PosResult [포즈 indx]
                                    print("Pose ", PosResult)

                                    structlist.append(PosResult)

                                else:
                                    SendMessage(socket_in_list, df_MESS_PY_RECV_ERROR) #없는 메시지
                                    
                                    i = 0

                                # 응답 메세지 전송
                                strbuff = ''
                                nPacketSize = 0
                                for i in range(0, len(structlist)) :
                                    bufTmp = struct.pack('i',structlist[i])
                                    nPacketSize += 4
                                    strbuff += bufTmp
                                    
                                #최초 12바이트를 헤더 생성
                                nPacketSize = 12 + nPacketSize
                                #패킷 총 사이즈 전송
                                bufTmp = struct.pack('iii',nPacketSize,nMessID,0)                                
                                SendData = bufTmp + strbuff                   
                                                                                        
                                    #한개씩 데이터 전송                             
                                for i in range(0, nPacketSize, BUFSIZE ) :      
                                    socket_in_list.send(SendData[i:i + BUFSIZE])
                                    time.sleep( 0.005 ) 
                                
                                print('[INFO][%s] Send Data From packet Size : %d' % (ctime(),  nPacketSize))                                                                
                           
                            except Exception:
                                print('Error')
                                SendMessage(socket_in_list, df_MESS_PY_RECV_ERROR)
                                #socket_in_list.close()
                                #connection_list.remove(socket_in_list)
                                continue
                else:
                    connection_list.remove(sock)
                    sock.close()
                    print('[INFO][%s] Dissconect user.' % ctime())
    except KeyboardInterrupt:
        # Close
        serverSocket.close()
        sys.exit()


