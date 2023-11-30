from collections import defaultdict
input = "smnpbnnaaaaa$a"
arr = [input]

for i in range(len(input)-1):
    tmp = input[-1] + input[:-1]
    arr.append(tmp)
    input = tmp

arr = sorted(arr)
print("Result arr: ", arr)

res = ""
for s in arr:
    res  += s[-1]
    
print("Result string: ", res)

BWT_dict = defaultdict(list)

for i in range(len(arr)):
    BWT_dict[arr[i][0]].append(i)
    
print("BWT Dict: ", BWT_dict)
    
L2F = []
index_desk = {}
for c in res:
    if c not in index_desk:
        L2F.append(BWT_dict[c][0])
        index_desk[c] = 1
    else:
        L2F.append(BWT_dict[c][index_desk[c]])
        index_desk[c] += 1
        
print(L2F)

