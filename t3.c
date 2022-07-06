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

    struct PCB *next_queue;
    struct PCB *next_total;
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

    p->next_queue = NULL;
    p->next_total = NULL;

    return p;
}


void print_states(int time, struct PCB* total)
{
    struct PCB* aux = (struct PCB*) malloc(sizeof(struct PCB));
    aux = total->next_total;
    printf("<%d> <%s> ", time, total->estado);

    while(aux !=NULL)
    {        
        
        printf("<%s> ", aux->estado);
        aux = aux->next_queue;
        
    }
    printf("\n");
    free(aux);
}

void processor(FILE* fin, FILE* fout)
{
    int qtde_proc = get_int(fin);
    int qtde_disp = get_int(fin);
   
    int* v_disp = get_disp(fin,qtde_disp);
    int cont_proc = 0;

    struct PCB *total = NULL;
    struct PCB *new = NULL;
    struct PCB *hnew = NULL;
    struct PCB *running = NULL;
    struct PCB *ready = NULL;
    struct PCB *pointer1 = NULL;
    struct PCB *pointer2 = NULL;


    int time = 0;
    int started_running = time;
    int next = get_int(fin);

    while(time < 5)
    {
        if(next == time)
        {
            if(new == NULL)
            {
                new = make_PCB(fin,cont_proc);
                pointer1 = new;
            }
            else
            {
                pointer1 = new;
                while(pointer1->next_queue != NULL)
                {
                    pointer1 = pointer1->next_queue;
                }
                pointer1->next_queue = make_PCB(fin,cont_proc);
                pointer1 = pointer1->next_queue;
            }

            if(total == NULL)
            {
                total = pointer1;
            }
            else
            {
                pointer2 = total;
                while(pointer2->next_total != NULL)
                {
                    pointer2 = pointer2->next_total;
                }
                pointer2->next_total = pointer1;
            }

            
            cont_proc++;
            next = get_int(fin);

        }  
        
        
        print_states(time,total);
        
        
        
        time++;  

    }
    
    fprintf(fout, "Quantidade de processos: %d\n", qtde_proc);
    fprintf(fout, "Quantidade de dispositivos: %d\n", qtde_disp);


    free(v_disp);
}

void main()
{
    FILE* fin = get_file("in.txt","r");
    FILE* fout = get_file("out.txt","w");

    processor(fin,fout);

    fclose(fin);
    fclose(fout);
}