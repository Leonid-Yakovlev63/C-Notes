#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLDBLACK   "\033[1m\033[30m" 
#define BOLDRED     "\033[1m\033[31m"
#define BOLDGREEN   "\033[1m\033[32m"
#define BOLDYELLOW  "\033[1m\033[33m"
#define BOLDBLUE    "\033[1m\033[34m"
#define BOLDMAGENTA "\033[1m\033[35m"
#define BOLDCYAN    "\033[1m\033[36m"
#define BOLDWHITE   "\033[1m\033[37m"


struct Note
{
    int64_t id;
    char title[256];
    char content[1024];
} Note;

static inline bool is_int (char* value) {
    return value == (int64_t)value ;
};

static inline int count_files_in_notes(){
    int file_count = 0;
    int dir_count = 0;
    int total = 0;

    DIR * dirp;
    struct dirent * entry;


    dirp = opendir("notes"); 
    while ((entry = readdir(dirp)) != NULL) {
        total++;
        if (entry->d_type == DT_REG) {
            file_count++;
        }
    }
    dir_count = total - file_count;
    closedir(dirp);
    return file_count;
}

int save_note(char * filename, struct Note *n)
{
    FILE * fn;
    char *c;
    int size = sizeof(struct Note);
  
    fn = fopen(filename, "wb");
    if (!fn)
    {
        printf("Error occured while opening file \n");
        return 1;
    }
    c = (char *)n;
    for (int i = 0; i < size; i++)
    {
        putc(*c++, fn);
    }
    fclose(fn);
    return 0;
}

void print_note(struct Note note)
{
    printf(GREEN"\nID: %d\nTitle: %s\nContent: %s\n"RESET,note.id, &note.title, &note.content);
};

bool file_exists (char *filename) {
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}


int main()
{
    printf(BLUE"Welcome to C-Notes!\n");
    printf("Instructions for programm\n");
    printf("For show notes enter: 1\n");
    printf("For create new note enter: 2\n");
    printf("For exit enter: 0\n"RESET);

    int action = 1;
    while (action != 0)
    {
        printf("Select action: ");
        if(!scanf("%d", &action))
        {
            printf(RED"ERROR! Please type an integer\n"RESET);
            return -1;
        } 
        else 
        {
            getchar();
            switch (action)
            {
                case 1:
                    printf("\n");
                    printf(GREEN"Total notes: %d\n"RESET, count_files_in_notes());

                    if (count_files_in_notes() != 0)
                    {
                        for(int i = 0; i < count_files_in_notes()+1; i++)
                        {
                            char fullFileName[25];
                            snprintf(fullFileName, sizeof fullFileName, "%s%d%s","notes/note_data",i+1, ".bin" );
                            if (file_exists(fullFileName)){

                            FILE* file = fopen(fullFileName, "rb");
                            struct Note note;
                            while (fread(&note, sizeof(Note), 1, file) == 1) 
                            { 
                                print_note(note);
                            } 
                            fclose(file);  
                            } else {
                                continue;
                            }
                        }
                        printf("\n");
                    }
                    else
                    {
                        printf("\n");
                    }
                    break;
                case 2:
                    struct Note *newNote = malloc(sizeof(Note));
                    

                    printf("Write title:\n");
                    fgets(newNote->title, 40, stdin);
                    newNote->title[strcspn(newNote->title, "\n")] = 0;

                    printf("Write content:\n");
                    fgets(newNote->content, 100, stdin);
                    newNote->content[strcspn(newNote->content, "\n")] = 0;


                    char fullFileName[25];
                    snprintf(fullFileName, sizeof fullFileName, "%s%d%s","notes/note_data",count_files_in_notes() + 1, ".bin" );
                    if(file_exists(fullFileName)){
                        snprintf(fullFileName, sizeof fullFileName, "%s%d%s","notes/note_data",count_files_in_notes() + 2, ".bin" );
                        newNote->id = count_files_in_notes()+2;
                    } else {
                        newNote->id = count_files_in_notes()+1;
                    }
                    save_note(fullFileName, newNote);
                    printf("\n");
                    printf(GREEN"Note created successfully!\n"RESET);
                    printf("\n");
                    free(newNote);
                    break;
                default:
                    printf("Unknown action!\n");
                break;
            };
        }
    }
    return 0;
};