# QUEUE_ESP32

//Comunicação inter-processos entre duas tarefas (task_send e task_receive) por meio de uma fila (queue).

// Ela alterna entre enviar "ligado" ou "desligado" para a fila e altera o estado de uma saída GPIO de acordo com o valor enviado para a fila. 

//Atrasos de 1000 milissegundos são adicionados no final do loop para retardar a execução.
