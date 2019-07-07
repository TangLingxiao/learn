#coding : utf-8

sequence = [1,2,3,4]


def gen_sub_sequence(sequence, index, temp):
    if index == len(sequence):
        print (temp)
        return

    #not have
    gen_sub_sequence(sequence, index + 1, temp)
    #have
    temp.append(sequence[index])
    gen_sub_sequence(sequence, index + 1, temp)
    temp.pop()


def all_permutations(sequence, index, temp, not_used):
    if index == len(sequence):
        #if len(temp) == len(sequence):
        print(temp)
        return
    
    for i in range(0, len(sequence)):
        if not_used[i] == True:
            #not use
            #all_permutations(sequence, index + 1, temp, not_used)
            #use
            not_used[i] = False
            temp.append(sequence[i])
            all_permutations(sequence, index + 1, temp, not_used)
            temp.pop()
            not_used[i] = True


def minimax(score):
    if len(score) == 1:
        return score[0]
    if len(score) == 2:
        return max(score[0], score[1])
    mid = len(score) / 2
    return max(minimax([score[i] for i in range(0, mid)]),
     minimax([score[i] for i in range(mid, len(score))]))



def is_safe(N, line, col, board):
    for i in range(0, N):
        if board[i][col] == 1:
            return False
        if board[line][i] == 1:
            return False
    for i, j in zip(range(line, -1, -1), range(col, -1, -1)):
        if board[i][j] == 1:
            return False
    for i, j in zip(range(line, -1, -1), range(col, len(board))):
        if board[i][j] == 1:
            return False
    return True

def n_queens(N, board, row):
    if row == N:
        print(board)
        return
    for j in range(0, N):
        if is_safe(N, row, j, board):
            board[row][j] = 1
            n_queens(N, board, row + 1)
            board[row][j] = 0
                

def sum_of_subsets(sequence, N, sum, set, index):
    if sum >= N:
        if sum == N:
            print(set)
        return
    if index == len(sequence):
        return

    for i in range(index, len(sequence)):
        if sequence[i] not in set:
            #sum_of_subsets(sequence, N, sum, set, index + 1)
            set.append(sequence[i])
            sum_of_subsets(sequence, N, sum + sequence[i], set, i)
            set.pop()

if __name__ == '__main__':
    #gen_sub_sequence(sequence, 0 , [])
    #all_permutations(sequence, 0, [], [True] * len(sequence))
    #print(minimax([10,20,30,550,403,2]))
    '''
    N = 8
    n_queens(N, [[0 for i in range(N)]for i in range(N)], 0)
    '''
    sum_of_subsets([1,2,3,4,5,6], 10, 0, [], 0)