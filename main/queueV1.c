#include <stdio.h>
#include "geralv2r1.h"
#include "gpiov2r3.h"



TaskHandle_t task_send_Handle = NULL;
TaskHandle_t task_receive_Handle = NULL;
QueueHandle_t queue;

void task_send(void *arg)                               //Essa é a tarefa que envia dados para a fila. Ela alterna entre enviar "ligado" ou "desligado" para a fila e altera o estado de uma saída GPIO de acordo com o valor enviado para a fila. Atrasos de 1000 milissegundos são adicionados no final do loop para retardar a execução.
{
   int output_state = 0;
   while(1) {
      // Se o estado da saída for 0, enviar "ligado" para a fila e alterar o estado para '0x0F'
      if (output_state == 0) {
         xQueueSend(queue, (void *)"ligado", pdMS_TO_TICKS(100));
         gpoDado(0x0F);
         output_state = 1;
      }
      // Se o estado da saída for 1, enviar "desligado" para a fila e alterar o estado para '0x00'
      else {
         xQueueSend(queue, (void *)"desligado", pdMS_TO_TICKS(100));
         gpoDado(0x00);
         output_state = 0;
      }
      vTaskDelay(pdMS_TO_TICKS(1000));
   }
}

void task_receive(void *arg)                                    //Essa é a tarefa que recebe dados da fila. Ele verifica continuamente a fila por novos dados e, se houver, imprime a mensagem recebida no console.
{
   char string_receive[9];
   while (1)
   {
      if ( xQueueReceive(queue, &(string_receive), pdMS_TO_TICKS(100)) ){
         printf("Recebido:  %s \n", string_receive);
      }
   }
}

void app_main(void)
{

    gpoIniciar();

   
   queue = xQueueCreate(5, 10);         // Criação da fila

   
   printf("\nDisponível no início: %d \n\n", uxQueueSpacesAvailable(queue));        // disponivel

   
   xTaskCreate(task_send, "task_send", 4096, NULL, 10, &task_send_Handle);      // Gravar dados na fila
   vTaskDelay(pdMS_TO_TICKS(1000));
   
   
   printf("Disponível depois de escrever: %d \n\n", uxQueueSpacesAvailable(queue));     // disonivel

   
   xTaskCreate(task_receive, "task_receive", 4096, NULL, 10, &task_receive_Handle);         // Ler dados da fila
   vTaskDelay(pdMS_TO_TICKS(1000));
   
   
   printf("\nDisponível no final: %d \n\n", uxQueueSpacesAvailable(queue));         // disponivel
}

