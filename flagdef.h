#define FREE 0
#define BLT 1
#define PLC 2
#define MAXSIZE 100

//blueteeth中断
int flag_bt;                        //for controller to check
int head4bt, rear4bt;
int head2bt, rear2bt;
char buf4bt[MAXSIZE][BT_SIZE];
char buf2bt[MAXSIZE][BT_SIZE];
//PLC
int flag_plc;                        //for controller to check
int head4plc, rear4plc;
int head2plc, rear2plc;
char buf4plc[MAXSIZE][PLC_SIZE];
char buf2plc[MAXSIZE][PLC_SIZE];
//Http
int flag_rec;                        //轮询到数据时置1，由主控制器读取检查
//int flag_sen;                        //需要发送时置1，由http发送线程检查
int bufID;
char bufrec[MAXSIZE];       //由主控读取
//char bufsen[MAXSIZE];      //由发送线程读取

//Lucas
void bt_init();
//LiwenYong
void PLC_init();
//QiuYingyue
void Send_init();
