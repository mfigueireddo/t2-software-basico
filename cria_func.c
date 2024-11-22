#include <stdio.h>
#include <stdlib.h>

#include "cria_func.h"

// Arrumar os códigos de exit
// Supondo que só exista um parâmetro variável
// Contar com sendo ponto flutuante?
void cria_func (void* f, DescParam params[], int n, unsigned char codigo[]){
   
    // Se forem recebidos mais parâmetros que o permitido
    if (n>3){
        fprintf(stderr, "Erro: foram passados mais parâmetro que o percebido.");
        exit(1);
    }

    // pushq %rbp
    *codigo++ = 0x55;

    // movq %rsp, %rbp
    *codigo++ = 0x48;
    *codigo++ = 0x89;
    *codigo++ = 0xe5;

    // Percorre os parâmetros recebidos
    for(int i=0; i<n; i++){
        
        // Se o parâmetro for inteiro
        if (params[i].tipo_val == INT_PAR){
           
            // Se o parâmetro for passado na chamada da função
            if (params[i].orig_val == PARAM){

                // movl %valorRdi, %valorRegistradorCorrespondente
                // Move o valor do parâmetro recebido para o registrador

                // movl
                *codigo++ = 0x89;

                // Se for o primeiro parâmetro
                if (i == 0) *codigo++ = 0xff; // %edi, %edi

                // Se for o segundo parâmetro
                else if (i == 1) *codigo++ = 0xfe; // %edi, %esi
                
                // Se for o terceiro parâmetro
                else if (i == 2) *codigo++ = 0xfa; // %edi, %edx

            }
            
            // Se o parâmetro for fixo
            else if (params[i].orig_val == FIX){

                // movl $valor, %valorRegistradorCorrespondente
                // Move o valor recebido em params para o registrador

                // Se for o primeiro parâmetro
                if (i == 0) *codigo++ = 0xbf; // %edi

                // Se for o segundo parâmetro
                else if (i == 1) *codigo++ = 0xbe; // %esi

                // Se for o terceiro parâmetro
                else if (i == 2) *codigo++ = 0xba; // %edx

                // $valor
                *(int*)codigo = params[i].valor.v_int; 
                codigo += sizeof(int);

            }
            
            // Se o parâmetro estiver no endereço de memória especificado no parâmetro
            else if (params[i].orig_val == IND){

                // movl *params.valor.v_ptr, %valorRegistradorCorrespondente
                // Move o valor recebido por um ponteiro em params para o registrador

                // Move o valor imediatamente depois para %rax
                *codigo++ = 0x48;
                *codigo++ = 0xb8;

                // Insere o conteúdo do ponteiro em %rax
                *(unsigned long*)codigo = (unsigned long)params[i].valor.v_ptr;
                codigo += sizeof(unsigned long*);

                // movl
                *codigo++ = 0x8b;

                /// Se for o primeiro parâmetro
                if (i == 0) *codigo++ = 0x38; // %edi

                // Se for o segundo parâmetro
                else if (i == 1) *codigo++ = 0x30; // %esi

                // Se for o terceiro parâmetro
                else if (i == 2) *codigo++ = 0x10; // %edx

            }
            
            // Caso de erro
            else {
                fprintf(stderr, "Erro: origem de parâmetro não conhecida.");
                exit(1);
            }
        
        }
        
        // Se o parâmetro for ponteiro
        else if(params[i].tipo_val == PTR_PAR){

            // Se o parâmetro for passado na chamada da função
            if (params[i].orig_val == PARAM){
            
                // %ptrRegistrador = %ptrRegistrador
                // Move um ponteiro recebido para o registrador

                // movq
                *codigo++ = 0x48;
                *codigo++ = 0x89;

                // Se for o primeiro parâmetro
                if (i == 0) *codigo++ = 0xff; // %rdi, %rdi

                // Se for o segundo parâmetro
                if (i == 1) *codigo++ = 0xfe; // %rdi, %rsi

                // Se for o terceiro parâmetro
                if (i == 2) *codigo++ = 0xfa; // %rdi, %rdx

            }
            
            // Se o parâmetro for fixo
            else if (params[i].orig_val == FIX){
                
                // *params[i].valor.vlr_ptr = %valorRegistrador
                // Move o valor do ponteiro em params para o registrador

                // Move o valor imediatamente depois para %rax
                *codigo++ = 0x48;
                *codigo++ = 0xb8;

                // Insere o conteúdo do ponteiro em %rax
                *(unsigned long*)codigo = (unsigned long)params[i].valor.v_ptr;
                codigo += sizeof(unsigned long*);

                // movl
                *codigo++ = 0x8b;

                // Se for o primeiro parâmetro
                if (i == 0) *codigo++ = 0x38; // (%rax), %edi

                // Se for o segundo parâmetro
                if (i == 1) *codigo++ = 0x30; // (%rax), %esi

                // Se for o terceiro parâmetro
                if (i == 2) *codigo++ = 0x10; // (%rax), %edx
            }
            
            // Se o parâmetro estiver no endereço de memória especificado no parâmetro
            else if (params[i].orig_val == IND){
                
                // Move o ponteiro recebido em params para o registrador
                // movq params[i].orig.valor.vlr_ptr = %ponteiroRegistrador

                // Move o valor imediatamente depois para %rax
                *codigo++ = 0x48;
                *codigo++ = 0xb8;

                // Insere o ponteiro em %rax
                *(unsigned long*)codigo = (unsigned long)params[i].valor.v_ptr;
                codigo += sizeof(unsigned long*);
                    
                // movq
                *codigo++ = 0x48;
                *codigo++ = 0x89;

                // Se for o primeiro parâmetro
                if (i == 0) *codigo++ = 0xc7; // %rax, %rdi

                // Se for o segundo parâmetro
                if (i == 1) *codigo++ = 0xc6; // %rax, %rsi

                // Se for o terceiro parâmetro
                if (i == 2) *codigo++ = 0xc2; // %rax, %rdx
            }
            
            // Caso de erro
            else {
                fprintf(stderr, "Erro: origem de parâmetro não conhecida.");
                exit(1);
            }
        
        }
        
        // Caso de erro
        else{
            fprintf(stderr, "Erro: tipo de parâmetro não conhecido.");
            exit(1);
        }
    
    }

    // movq
    *codigo++ = 0x48;
    *codigo++ = 0xb8;
    // Passa o endereço da função para o registrador
    *(unsigned long*)codigo = (unsigned long)f;
    codigo += sizeof(unsigned long*);

    // Aciona a função
    *codigo++ = 0xff;
    *codigo++ = 0xd0;

    // Finalização
    *codigo++ = 0xc9;
    *codigo++ = 0xc3;
}