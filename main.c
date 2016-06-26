#include <stdio.h>
#include <string.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>

int main(int argc, char *argv[])
{
#define MAX_SIZE 80
#define MQ_NAME "/test.mq"
  char buffer[MAX_SIZE];
  bzero(buffer, sizeof(char) * MAX_SIZE);

  struct mq_attr mqattr;
  bzero(&mqattr, sizeof(struct mq_attr));

  mqattr.mq_flags = 0;
  mqattr.mq_maxmsg = 10;
  mqattr.mq_msgsize = MAX_SIZE;
  mqattr.mq_curmsgs = 0;

  mqd_t mqdes = mq_open(MQ_NAME, (O_CREAT | O_RDWR), 0666, &mqattr);
  if(mqdes == (mqd_t)-1)
  {
    perror("Ошибка при создании очереди: ");
    return -1;
  }

  ssize_t rd = mq_receive(mqdes, buffer, sizeof(char) * MAX_SIZE, NULL);
  if(rd <= 0)
    perror("Ошибка чтения: ");
  else
  {
    FILE *fp = fopen("/home/box/message.txt", "w");
    if(fp == NULL)
      perror("Ошибка при открытии файла: ");
    fprintf(fp, "%s", buffer);
    fclose(fp);
  }

  mq_close(mqdes);
  mq_unlink(MQ_NAME);
  return 0;
}
