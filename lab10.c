#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{	
    struct Trie* childNode[26];
    int leaf;
    int count;
};


// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie* node = (struct Trie*) calloc (1, sizeof(struct Trie));
    for (int i = 0; i < 26; i++)
        node->childNode[i] = NULL;
    node->leaf = 0;
    node->count = 0;
    return node;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct Trie * temp = pTrie;

    for(int i = 0; word[i] != '\0'; i++ )
    {
        int index = (int) word[i] - 'a';
        if (temp->childNode[index] == NULL )
        {
            temp->childNode[index] = createTrie();
        }
        temp = temp->childNode[index];
    }
    temp->leaf = 1;
    temp->count += 1;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct Trie *temp = pTrie;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = (int) word[i] - 'a';
        if (temp->childNode[index] == NULL)
            return 0; // Word not found
        temp = temp->childNode[index];
    }
    if (temp->leaf) {
        return temp->count;
    } else {
        return 0;
    }

}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
     if (pTrie) 
     {
        for (int i = 0; i < 26; i++) {
            
            if (pTrie->childNode[i]) 
            {
                deallocateTrie(pTrie->childNode[i]);
            }
        }

        free(pTrie);
    }

    return NULL;
}


// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        fprintf(stderr, "Failed to open the file\n");
        return 0;
    }

    int numWords;
    fscanf(file, "%d\n", &numWords);

    char buffer[256];
    int wordCount = 0;

    while (fgets(buffer, sizeof(buffer), file) && wordCount < numWords)
    {
        buffer[strcspn(buffer, "\n")] = 0;
        pInWords[wordCount] = strdup(buffer);
        wordCount++;
    }

    fclose(file);
    return wordCount;

}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}