#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define RED 0
#define BLACK 1

struct node{
    char *word;
    int c;
    int view;
    struct node *parent;
    struct node *left;
    struct node *right;
};

struct bstnode{
    char *word;
    struct bstnode *parent;
    struct bstnode *left;
    struct bstnode *right;
};

int Count = 0;
struct node *ROOT;
struct node *NILL;

struct bstnode *BSTWORD;
struct bstnode *NILLBST;

int fast_compare( const char *ptr0, const char *ptr1, int len ){
    int fast = len/sizeof(size_t) + 1;
    int offset = (fast-1)*sizeof(size_t);
    int current_block = 0;

    if( len <= sizeof(size_t)){ fast = 0; }


    size_t *lptr0 = (size_t*)ptr0;
    size_t *lptr1 = (size_t*)ptr1;

    while( current_block < fast ){
        if( (lptr0[current_block] ^ lptr1[current_block] )){
            int pos;
            for(pos = current_block*sizeof(size_t); pos < len ; ++pos ){
                if( (ptr0[pos] ^ ptr1[pos]) || (ptr0[pos] == 0) || (ptr1[pos] == 0) ){
                    return  (int)((unsigned char)ptr0[pos] - (unsigned char)ptr1[pos]);
                }
            }
        }

        ++current_block;
    }

    while( len > offset ){
        if( (ptr0[offset] ^ ptr1[offset] )){
            return (int)((unsigned char)ptr0[offset] - (unsigned char)ptr1[offset]);
        }
        ++offset;
    }


    return 0;
}

void left_rotate(struct node *x){
    struct node *y;


    y = x->right;
    x->right = y->left;
    if(y->left != NILL){
        y->left->parent = x;
    }


    y->parent = x->parent;
    if(y->parent == NILL){
        ROOT = y;
    }
    else if(x == x->parent->left){
        x->parent->left = y;
    }
    else{
        x->parent->right = y;
    }


    y->left = x;
    x->parent = y;
}

void right_rotate(struct node *x){
    struct node *y;


    y = x->left;
    x->left = y->right;
    if(y->right != NILL){
        y->right->parent = x;
    }


    y->parent = x->parent;
    if(y->parent == NILL){
        ROOT = y;
    }
    else if(x == x->parent->left){
        x->parent->left = y;
    }
    else{
        x->parent->right = y;
    }


    y->right = x;
    x->parent = y;
}

void red_black_insert_fixup(struct node *z){
    while(z->parent->c == RED){


        if(z->parent == z->parent->parent->left){

            if(z->parent->parent->right != NULL && z->parent->parent->right->c == RED){
                z->parent->c = BLACK;
                z->parent->parent->right->c = BLACK;
                z->parent->parent->c = RED;
                z = z->parent->parent;
            }


            else{


                if(z == z->parent->right){
                    z = z->parent;
                    left_rotate(z);
                }

                z->parent->c = BLACK;
                z->parent->parent->c = RED;
                right_rotate(z->parent->parent);
            }
        }


        else{


            if(z->parent->parent->left != NULL && z->parent->parent->left->c == RED){
                z->parent->c = BLACK;
                z->parent->parent->left->c = BLACK;
                z->parent->parent->c = RED;
                z = z->parent->parent;
            }


            else{

                if(z == z->parent->left){
                    z = z->parent;
                    right_rotate(z);
                }

                z->parent->c = BLACK;
                z->parent->parent->c = RED;
                left_rotate(z->parent->parent);
            }
        }
    }

    ROOT->c = BLACK;
}

void red_black_insert(char word[], int length){
    struct node *z, *x, *y;

    z = malloc(sizeof(struct node)+ sizeof(char)* (length+1));
    z->word = ((char *) z) + sizeof(*z);
    strcpy(z->word, word);
    z->c = RED;
    z-> view = 1;
    z->left = NILL;
    z->right = NILL;

    x = ROOT;
    y = NILL;

    while(x != NILL){
        y = x;
        if(fast_compare(x->word,z->word, length) > 0){
            x = x->left;
        }
        else{
            x = x->right;
        }
    }

    if(y == NILL){
        ROOT = z;
    }
    else if(fast_compare(y->word,z->word,length) > 0){
        y->left = z;
    }
    else{
        y->right = z;
    }

    z->parent = y;

    red_black_insert_fixup(z);
}

int bst_search(struct bstnode* root, char word[], int length){

    while (root != NILLBST) {

        if (fast_compare(root -> word, word,length) < 0)
            root = root->right;

        else if (fast_compare(root -> word, word, length) > 0)
            root = root->left;
        else
            return 1;
    }
    return 0;
}

void bst_insert(char *word, int length){

    struct bstnode *y, *z, *x;

    z = malloc(sizeof(struct bstnode) + sizeof(char)* (length+1));
    z->word = ((char *) z) + sizeof(*z);
    strcpy(z->word, word);
    z->left = NILLBST;
    z->right = NILLBST;

    y = NILLBST;
    x = BSTWORD;

    while(x != NILLBST){
        y = x;
        if(fast_compare(z->word, x->word, length) < 0)
            x = x->left;
        else
            x = x ->right;
    }

    z->parent = y;

    if(y ==  NILLBST)
        BSTWORD = z;
    else{
        if (fast_compare(z->word, y->word, length) < 0)
            y->left = z;
        else
            y->right = z;

    }

}

void bstprint(struct bstnode *root){
    struct bstnode *current, *pre;

    if (root == NILLBST)
        return;

    current = root;
    while (current != NILLBST) {

        if (current->left == NILLBST) {
            printf("%s\n", current->word);
            current = current->right;

        }
        else {


            pre = current->left;
            while (pre->right != NILLBST
                   && pre->right != current)
                pre = pre->right;


            if (pre->right == NILLBST) {
                pre->right = current;
                current = current->left;
            }

            else {
                pre->right = NILLBST;
                printf("%s\n", current->word);
                current = current->right;
            }
        }
    }
}

int count(struct node* node){

    if (node == NILL)
        return 0;

    int count = 0;
    struct node *current, *pre;

    current = node;
    while (current != NILL) {

        if (current->left == NILL) {
            if(current -> view == 1)
                count++;
            current = current->right;

        }
        else {


            pre = current->left;
            while (pre->right != NILL
                   && pre->right != current)
                pre = pre->right;


            if (pre->right == NILL) {
                pre->right = current;
                current = current->left;
            }

            else {
                pre->right = NILL;
                if(current -> view == 1)
                    count++;
                current = current->right;
            }
        }
    }
    return count;
}

void wordInsert(struct bstnode *root, char inputString[], char selectedWord[], int length){

    if(root == NILLBST)
        return;

    wordInsert(root -> left, inputString, selectedWord,length);

    int check = 1;
    if (fast_compare(root->word, selectedWord, length) != 0) {

        for (int i = 0; i < length; i++) {

            int counter = 0;
            int countersel = 0;
            int counterinp = 0;

            // 2. posto giusto
            if (inputString[i] == selectedWord[i]) {
                if (root->word[i] != inputString[i]) {
                    check = 0;
                    break;
                }
            } else {
                // 3. posto in cui non puo comparire
                if (root->word[i] == inputString[i]) {
                    check = 0;
                    break;
                }
            }

            for (int t = 0; t < length; t++) {

                if (root->word[t] == inputString[i])
                    counter++;

                if (selectedWord[t] == inputString[i])
                    countersel++;

                if (inputString[t] == inputString[i])
                    counterinp++;
            }

            // 1. simbolo non appartiene
            if (countersel == 0)
                if (counter != 0) {
                    check = 0;
                    break;
                }

            // 4. numero minimo di volte che compare
            if (countersel >= counterinp) {
                if (counterinp > counter) {
                    check = 0;
                    break;
                }
            } else {
                if (countersel != counter) {
                    check = 0;
                    break;
                }
            }
        }
    }
    if(check == 1)
        red_black_insert(root->word,length);

    wordInsert(root -> right, inputString, selectedWord,length);

}

void wordCheck(char inputString[], char selectedWord[], int length) {
    char *result = calloc(sizeof(char),(length + 1));

    for (int i = 0; i < length; i++) {
        int counter = 0;
        int totalPres = 0;
        int plusCounter = 0;
        int counterSel = 0;
        int plusCounterSel = 0;

        if (inputString[i] == selectedWord[i]) {
            result[i] = '+';
        } else {


            for (int t = 0; t < length; t++) {
                if (inputString[i] == selectedWord[t]) {
                    totalPres++;
                }

                if (inputString[i] == inputString[t]) {
                    counter++;
                    if (inputString[t] == selectedWord[t]) {
                        plusCounter++;
                    }
                }

                if (t <= i) {
                    if (inputString[i] == inputString[t]) {
                        counterSel++;
                        if (inputString[t] == selectedWord[t])
                            plusCounterSel++;
                    }
                }

            }

            if (totalPres == 0) {
                result[i] = '/';


            } else {
                if (totalPres >= counter) {
                    result[i] = '|';
                } else {
                    if (counterSel <= totalPres && (counterSel - plusCounterSel) <= (totalPres - plusCounter)) {
                        result[i] = '|';
                    } else {
                        result[i] = '/';
                    }

                }
            }
        }
    }

    printf("%s\n", result);
    free(result);
}

void wordViewChange(struct node *root, char inputString[], char selectedWord[],int length){

    if(root == NILL)
        return;

    wordViewChange(root->left,  inputString, selectedWord,  length);

    if(root->view == 1) {
        if (fast_compare(root->word, selectedWord, length) != 0) {

            for (int i = 0; i < length; i++) {

                int counter = 0;
                int countersel = 0;
                int counterinp = 0;

                // 2. posto giusto
                if (inputString[i] == selectedWord[i]) {
                    if (root->word[i] != inputString[i]) {
                        root->view = 0;
                        Count--;
                        break;
                    }
                } else {
                    // 3. posto in cui non puo comparire
                    if (root->word[i] == inputString[i]) {
                        root->view = 0;
                        Count--;
                        break;
                    }
                }


                for (int t = 0; t < length; t++) {

                    if (root->word[t] == inputString[i])
                        counter++;

                    if (selectedWord[t] == inputString[i])
                        countersel++;

                    if (inputString[t] == inputString[i])
                        counterinp++;
                }

                // 1. simbolo non appartiene
                if (countersel == 0)
                    if (counter != 0) {
                        root->view = 0;
                        Count--;
                        break;
                    }


                // 4. numero minimo di volte che compare
                if (countersel >= counterinp) {
                    if (counterinp > counter) {
                        root->view = 0;
                        Count--;
                        break;
                    }
                } else {
                    if (countersel != counter) {
                        root->view = 0;
                        Count--;
                        break;
                    }
                }
            }

        }
    }

    wordViewChange(root->right,inputString,selectedWord,length);
}

void deleteTree(struct node* root){

    struct node *y, *x;
    y = root -> left;
    x = root -> right;
    if (root == NILL)
        return;
    deleteTree(y);

    deleteTree(x);
    free(root);
}

void inorder(struct node* root){
    struct node *current, *pre;

    if (root == NILL)
        return;

    current = root;
    while (current != NILL) {

        if (current->left == NILL) {
            if(current -> view == 1)
                printf("%s\n", current->word);
            current = current->right;

        }
        else {


            pre = current->left;
            while (pre->right != NILL
                   && pre->right != current)
                pre = pre->right;


            if (pre->right == NILL) {
                pre->right = current;
                current = current->left;
            }

            else {
                pre->right = NILL;
                if(current -> view == 1)
                    printf("%s\n", current->word);
                current = current->right;
            }
        }
    }
}


int main() {
    NILL = malloc(sizeof(struct node));
    NILL->c = BLACK;
    NILLBST = malloc(sizeof(struct bstnode));

    ROOT = NILL;
    BSTWORD = NILLBST;
    int scanfint = 0;
    int wordFound = 0;
    int f = 0;
    int numTry = 0;

    scanfint = scanf("%d", &f);
    int n = f + 20;
    int k = f + 1;

    char inputString[n];
    char selectedword[k];
    do{
        scanfint = scanf("%s", inputString);
        if(inputString[0] != '+')
            bst_insert(inputString,f);
        if (strcmp(inputString, "+inserisci_inizio") == 0) {
            do {
                scanfint = scanf("%s", inputString);
                if (inputString[0] != '+')
                    bst_insert(inputString,f);
            } while (inputString[0] != '+');
        }

    }while(strcmp(inputString, "+nuova_partita"));

    do {
        scanfint = scanf("%s", selectedword);

        scanfint = scanf("%d", &numTry);

        char wordInput[numTry][k];

        for (int i = 0; i < numTry; i++) {
            scanfint = scanf("%s", inputString);

            if (strcmp(inputString, "+stampa_filtrate") == 0) {
                if(i == 0)
                    bstprint(BSTWORD);
                else
                    inorder(ROOT);
                i--;
            }


            if (strcmp(inputString, "+inserisci_inizio") == 0) {
                do {
                    scanfint = scanf("%s", inputString);
                    if (inputString[0] != '+') {
                        bst_insert(inputString,f);

                        if (i > 0) {
                            int check = 1;
                            for (int w = 0; w < i; w++) {
                                for (int j = 0; j < f; j++) {

                                    int counter = 0;
                                    int countersel = 0;
                                    int counterinp = 0;

                                    // 2. posto giusto
                                    if (wordInput[w][j] == selectedword[j]) {
                                        if (wordInput[w][j] != inputString[j]) {
                                            check = 0;
                                            break;
                                        }
                                    } else {
                                        // 3. posto in cui non puo comparire
                                        if (wordInput[w][j] == inputString[j]) {
                                            check = 0;
                                            break;
                                        }
                                    }

                                    for (int t = 0; t < f; t++) {

                                        if (inputString[t] == wordInput[w][j])
                                            counter++;

                                        if (selectedword[t] == wordInput[w][j])
                                            countersel++;

                                        if (wordInput[w][t] == wordInput[w][j])
                                            counterinp++;
                                    }

                                    // 1. simbolo non appartiene
                                    if (countersel == 0)
                                        if (counter != 0) {
                                            check = 0;
                                            break;
                                        }

                                    // 4. numero minimo di volte che compare
                                    if (countersel >= counterinp) {
                                        if (counterinp > counter) {
                                            check = 0;
                                            break;
                                        }
                                    } else {
                                        if (countersel != counter) {
                                            check = 0;
                                            break;
                                        }
                                    }
                                }
                                if(check == 0)
                                    break;
                            }
                            if(check == 1){
                                Count ++;
                                red_black_insert(inputString,f);}

                        }
                    }

                } while (inputString[0] != '+');
                i--;
            }

            if (i == 0) {
                if (bst_search(BSTWORD, inputString,f) == 1) {
                    if (strcmp(selectedword, inputString) == 0) {
                        printf("ok\n");
                        i = numTry + 1;
                        wordFound = 1;
                    } else {
                        strcpy(wordInput[i], inputString);
                        wordCheck(inputString, selectedword, f);
                        wordInsert(BSTWORD, inputString, selectedword, f);
                        Count = count(ROOT);
                        printf("%d\n", Count);
                    }

                } else {
                    if (inputString[0] != '+') {
                        printf("not_exists\n");
                        i--;
                    }
                }
            } else {
                if (bst_search(BSTWORD, inputString,f) == 1) {
                    if (strcmp(selectedword, inputString) == 0) {
                        printf("ok\n");
                        i = numTry + 1;
                        wordFound = 1;
                    } else {
                        strcpy(wordInput[i], inputString);
                        wordCheck(inputString, selectedword, f);
                        wordViewChange(ROOT, inputString, selectedword, f);
                        printf("%d\n", Count);
                    }

                } else {
                    if (inputString[0] != '+') {
                        printf("not_exists\n");
                        i--;
                    }
                }
            }
        }

        if (wordFound == 0)
            printf("ko\n");

        wordFound = 0;

        deleteTree(ROOT);

        ROOT = NILL;

        do{
            scanfint = scanf("%s", inputString);

            if (strcmp(inputString, "+inserisci_inizio") == 0) {
                do {
                    scanfint = scanf("%s", inputString);
                    if (inputString[0] != '+')
                        bst_insert(inputString,f);
                } while(inputString[0] != '+');
            }
            if(scanfint == EOF){
                break;
            }

        }while(strcmp(inputString, "+nuova_partita"));

    }while(!strcmp(inputString, "+nuova_partita"));

    scanfint ++;

    return 0;
}
