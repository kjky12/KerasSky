# -*- coding: utf8 -*-
'''Train a simple deep NN on the MNIST dataset.

Get to 98.40% test accuracy after 20 epochs
(there is *a lot* of margin for parameter tuning).
2 seconds per epoch on a K520 GPU.
'''

from __future__ import print_function
import numpy as np
np.random.seed(1337)  # for reproducibility


from keras.models import Sequential
from keras.layers.core import Dense, Dropout, Activation
from keras.optimizers import SGD, Adam, RMSprop
from keras.utils import np_utils

import LoadPoseK
import Readini as rdini



def Lerning( ): #학습 
    global X_train, X_test, Y_train, Y_test, model, rms, score, batch_size, nb_epoch, nb_classes

    batch_size = 128
    nb_classes = 0      # sean : number of class   - 파일의 종류에 따라 값이 올라감(0.csv~ 9.csv : 10개)
    nb_epoch = 20       # sean : tunning parameter   - sean이라는 값이 뭔가 얻은 값을 넣어줘야할거 같은데 정확히 판단 안됨..

    # the data, shuffled and split between train and test sets
    (X_train, y_train), (X_test, y_test), nb_classes = LoadPoseK.load_data()     #파일을 읽어드림..(혹시 폴더 내용 및 파일 갯수가 바뀌어도 그냥 실행시키면 다 읽을것으로 판단됨)

    #X_train : 전체 데이터..?
    #x_test :  테스트 데이터..?

    #y_train : Y_train를 만드는데 사용..
    #y_train : Y_test를 만드는데 사용..

    X_train = X_train.astype('float32')
    X_test = X_test.astype('float32')
    X_train /= 360
    X_test /= 360

    print(X_train.shape, 'train samples')
    print(X_test.shape, 'test samples')

    input_shape_Value = X_train.shape[1]    #이 값으로 나중에 model.add 할때 넣어줘야합니다.

    # convert class vectors to binary class matrices
    
    Y_train = np_utils.to_categorical(y_train, nb_classes)
    print (Y_train[0])
    Y_test = np_utils.to_categorical(y_test, nb_classes)
    

    model = Sequential()

    FieldCnt = int(rdini.ReadConfig('config.ini', 'POSE_FIELD_CNT'))     #ini config 파일을 읽어서 값을 넣는다.

    print(FieldCnt)
    
    model.add(Dense(FieldCnt, input_shape=(input_shape_Value,))) # input_shape_Value : X_train.shape값을 맞춰야합니다.    
    model.add(Activation('relu'))
    model.add(Dropout(0.5))

    model.add(Dense(FieldCnt))
    model.add(Activation('relu'))
    model.add(Dropout(0.4))

    model.add(Dense(FieldCnt))
    model.add(Activation('relu'))
    model.add(Dropout(0.3))

    model.add(Dense(FieldCnt))
    model.add(Activation('relu'))
    model.add(Dropout(0.2))

    model.add(Dense(nb_classes))
    model.add(Activation('softmax'))
    

    rms = RMSprop()
    model.compile(loss='categorical_crossentropy', optimizer=rms)
    
    #fit : epoch숫자로 모델 트레이닝!
    model.fit(X_train, Y_train,
              batch_size=batch_size,nb_epoch=nb_epoch,
              verbose=2,
              #validation_data=(X_train, Y_train))
              validation_data=(X_test, Y_test)) #에러 발생..
    #오류에 "show_accuracy=True" 더이상 사용하지 않는다고해서 삭제해줬다.    
    #model.fit(X_train, Y_train,
    #          batch_size=batch_size, nb_epoch=nb_epoch,
    #          show_accuracy=True, verbose=2,
    #          validation_data=(X_test, Y_test))            이부분이 x_test,ytest를 테스트로 넣는다는건데 안넣어도된다.

    #--------------------------------------------------------------------------------------------
    #evaluate : 손실률 계산... score가 손실률인듯..
    #리턴 : Scalar test loss or list of scalars 
    #--------------------------------------------------------------------------------------------
    score = model.evaluate(X_test, Y_test, verbose=0)
    #오류에 "show_accuracy=True" 더이상 사용하지 않는다고해서 삭제해줬다.
    #score = model.evaluate(X_test, Y_test, show_accuracy=True, verbose=0)
    
    

#자세 인식 
def PoseCognition(X_value):
    #Y_result = model.predict(X_test)

    #여기서 세이브를하면 뭔가 파일이 생김... 혹시 다시 재실행할경우 overwrite할지 여부를 물어봄(파일 삭제시 정상 동작)
    #model.save_weights("trained_model")
    try:
        #여기서 데이터를 predict에 들어갈 데이터로 바꿔줘야하는데(아마 하단에 만들어 놓은대로 하면 바뀔거같음 전체 테스트 안해봄)
        #numpy.arry 타입..

        X_numpyArray = np.array(X_value)           #이렇게 넣으면된다!!(들어가는지 부부 부분 테스트 해봄)
        X_numpyArray = X_numpyArray.astype('float32')   #이건 메인 소스 위에도 float32형으로 맞춰준듯하여 추가함
        #model.load_weights("trained_model")

        #--------------------------------------------------------------------------------------------
        #predict
        #입력 샘플에 대한 출력 예측을 생성하고 일괄 처리 방식으로 샘플을 처리합니다.
        #리턴 : A Numpy array of predictions.
        #--------------------------------------------------------------------------------------------
        X_numpyArray = np.array([X_numpyArray]) #혹시 에러가난다면 이부분을 주석!
        Y_result = model.predict(X_numpyArray)

        Y_resultMax = max(Y_result[0])
        Y_resultInx  = np.where(Y_result[0]== Y_resultMax)

        returnTmp = Y_resultInx
        return returnTmp[0][0]
    except Exception:
        return "Error!!"


