input = "CACAGATTACACACA"

word_list = []
for i in range(len(input)):
    word_list.append(input[i:])
    
index = range(len(input))
rank = zip(word_list, index)
rank = list(rank)
res = sorted(rank, key = lambda x: x[0])

for word, index in res:
    print(index)