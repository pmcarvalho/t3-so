#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

struct PCB
{
    int id;
    char* estado;
    int t_espera;
    int* info;
    int len_info;

    struct PCB *next;
};


FILE* get_file(const char* path, const char* mode)
{
    FILE* f;
    f = fopen(path,mode);
    if(f == NULL)
    {
        printf("Erro ao abrir arquivo!\n");
        exit(1);
    }

    return f;

}

int get_int(FILE* f)
{
    char c;
    int num = 0;
    while(!feof(f))
    {
        fscanf(f,"%c",&c);
        if(c >= 48 && c <= 57)
        {
            num = num*10 + c - 48;
        }
        else
        {
            break;
        }
    }

    return num;

}

int* get_disp(FILE* f, int quant)
{
    int* v = malloc(sizeof(int)*quant);

    for(int i = 0; i < quant; i++)
    {
        v[i] = get_int(f);
    }

    return v;
}

int* get_info(FILE* f, int* len)
{
    int* v = malloc(sizeof(int));
    int tam = 1;
    char c;
    while(true)
    {
        fscanf(f,"%c",&c);
        if(c == '\n')
        {
            break;
        }

        if(c != ' ')
        {
            v[tam-1] = v[tam-1]*10 + c - 48;
        }
        else
        {
            tam++;
            v = realloc(v,sizeof(int)*tam);
            v[tam-1] = 0;
        }

    }
    
    *len = tam;

    return v;
}

struct PCB* make_PCB(FILE* f, int pid)
{
    struct PCB* p = (struct PCB *) malloc(sizeof(struct PCB));

    p->id = pid;
    p->estado = "new";
    p->t_espera = 0;

    p->info = get_info(f,&p->len_info);

    p->next = NULL;

    return p;
}



void main()
{
    FILE* fin = get_file("in.txt","r");
    FILE* fout = get_file("out.txt","w");

    int qtde_proc = get_int(fin);
    int qtde_disp = get_int(fin);
   
    int* v_disp = get_disp(fin,qtde_disp);
    int cont_proc = 0;

    struct PCB *head = NULL;
    struct PCB *point = NULL;

    int time = 0;
    int nex = get_int(fin);

    while(time < 5)
    {
        if(nex == time)
        {
            if(cont_proc == 0)
            {
                head = make_PCB(fin,cont_proc);
            }
            else
            {
              
                if(cont_proc == 1)
                {
                    
                    head->next = make_PCB(fin,cont_proc);
                    point = head->next;
                }    
                else
                {
                    
                    point->next = make_PCB(fin,cont_proc);
                    point = point->next;
                    
                }
                
                
            }
            
            cont_proc++;
            nex = get_int(fin);

        }  
        
        
        for (int i = 0; i < cont_proc; i++)
        {
            
            if(i == 0)
            {
                printf("<%d> <%s> ", time, head->estado);
                point = head;
            }
            else if(point->next != NULL)
            {
                printf("<%s> ", point->estado);
                point = point->next;
            }
            
        }
        printf("\n");
        
        
        
        time++;  

    }
    
    fprintf(fout, "Quantidade de processos: %d\n", qtde_proc);
    fprintf(fout, "Quantidade de dispositivos: %d\n", qtde_disp);

    //printf("<%s> ", head->estado);

    free(v_disp);
    fclose(fin);
    fclose(fout);
}