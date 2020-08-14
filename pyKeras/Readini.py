
def ReadConfig(strFileName, strCategory) :
    try :
        f= open ("./" + strFileName, 'r')
        bFalg = True
        while True :
            line = f.readline()

            if not line :
                bFalg = False
                break

            if strCategory in line :
                line = f.readline()
                if not line :
                    bFalg = False
                    break
                
                line = line[line.find('=') + 1 : len(line)]
                value = line.strip()
                break            
        f.close()

        if(bFalg) :
            return value
        else :
            return "Can't find category"
            
    except Exception:
        return "Error!! please check filename"
    
