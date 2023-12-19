from collections import defaultdict

def bwt(str):
    
    """
    Input:
        str: String, the input of the string to be transformed throught BWT
    Output:
        res: String, the result of BWT of the input string
    """
    
    curStr = str
    allKinds = [curStr]

    for _ in range(len(curStr)-1):
        strTmp = curStr[-1] + curStr[:-1]
        allKinds.append(strTmp)
        curStr = strTmp

    allKinds = sorted(allKinds)

    res = ""
    for s in allKinds:
        res  += s[-1]
    return res

def invBWT(str):
    
    """
    Input:
        str: String, the input of the string to be transformed throught BWT
    Output:
        res: String, the result of inverse BWT of the input string
    """
    
    str_sort = sorted(str)
    BWT_dict = defaultdict(list)
    for i in range(len(str_sort)):
        BWT_dict[str_sort[i]].append(i)
    
    sortToInput = {} 
    index_desk = {}
    for index, c in enumerate(str):
        if c not in index_desk:
            sortToInput[BWT_dict[c][0]] = index
            index_desk[c] = 1
        else:
            sortToInput[BWT_dict[c][index_desk[c]]] = index
            index_desk[c] += 1
    
    cur = sortToInput[0]
    res = ""
    
    while len(res) != len(str):
        res += str[sortToInput[cur]]
        cur = sortToInput[cur]
        
    return res

input1 = "BANANA$"
input2 = "ANNB$AA"

bwtResult = bwt(input1)
invBwtResult = invBWT(input2)

print("BWT Result: ", bwtResult)
print("Inverse BWT Result: ", invBwtResult)

