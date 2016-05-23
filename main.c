#include “flagdef.h”

//blueteeth中断
//int flag_bt;                        //for controller to check
int head4bt = -1, rear4bt = -1;
int head2bt = -1, rear2bt = -1;
char buf4bt[MAXSIZE][BT_SIZE];
char buf2bt[MAXSIZE][BT_SIZE];
//PLC
//int flag_plc;                        //for controller to check
int head4plc = -1, rear4plc = -1;
int head2plc = -1, rear2plc = -1;
char buf4plc[MAXSIZE][PLC_SIZE];
char buf2plc[MAXSIZE][PLC_SIZE];
//Http
int flag_rec;                        //轮询到数据时置1，由主控制器读取检查
//int flag_sen;                        //需要发送时置1，由http发送线程检查
int bufID;
char bufrec[MAXSIZE];       //由主控读取
//char bufsen[MAXSIZE];      //由发送线程读取

/*  环形缓冲区API
     对于一个环形缓冲区，rear和head分别指着最新插入和最先插入的位置，初值为-1
*/

/*
 *   添加数据到缓冲区
 *  参数说明：
 *    buf：      缓冲区名字
 *    data：    插入数据
 *    head：   缓冲区头指针
 *    rear：     缓冲区尾指针
 *    size：     缓冲区大小
 *    返回值：TRUE表示插入成功，FALSE表示满了插入失败
 */
bool writebuf(char **buf, char *data, int *head, int *rear, int size){
    if(*rear==*head)      return false;
    *rear = *rear+1;
    if(*rear==size)          *rear = 0;
    strcpy(buf[rear], data);
    return true;
}

/*
 *   从缓冲区读取数据
 *  参数说明：
 *    buf：      缓冲区名字
 *    data：    读取数据的数组
 *    head：   缓冲区头指针
 *    rear：     缓冲区尾指针
 *    size：     缓冲区大小
 *    返回值：TRUE表示读取成功，FALSE表示满了读取失败（为空，一般来说先检查是否为空再读就好了）
 */
bool readbuf(char **buf, char *data, int *head, int *rear, int size){
    if(*rear==*head)      return false;
    *head = *head+1;
    if(*head==size)          *head = 0;
    strcpy(data, buf[head]);
    return true;
}

//主程序
int main(){
    //Controller Data Structure 控制器数据结构
    char buf[DEV_SIZE];
    int device_id;
    int device_type;
    bt_init();
    PLC_init();
    Send_init();
    while(1){
        //Deal with the blueteeth data recieve
        if(head4bt!=rear4bt){
            //Read data from blueteeth
            readbuf(buf4bt, buf, head4bt, rear4bt, BT_SIZE);
            //Get device ID

            //Send data to the server
            post(buf, device_id, server);
        }
        //Deal with the PLC data recieve
        if(head4plc!=head4plc){
            //Read data from PLC
            readbuf(buf4plc, buf, head4plc, rear4plc, PLC_SIZE);
            //Get device ID

            //Send data to the server
            post(buf, device_id, server);
        }
        //Deal with the HTTP data recieve
        if(flag_rec){
          //Get device ID and judge type

          //Send data to the ID
          switch(device_type){
              case PLC:   writebuf(buf2plc, bufrec, head2plc, rear2plc, PLC_SIZE); break;
              case BT:     writebuf(buf2bt,  bufrec, head2bt,   rear2bt,  BT_SIZE);   break;
          }
        }
    }
}
