#ifndef _PACKET_TYPE_H
#define _PACKET_TYPE_H

typedef enum empacket_type
{
	PACKET_TEST = 0x800,   //���԰�
	PACKET_LOGIN,          //��½����
	PACKET_ACK_LOGIN,      //��½�ظ���
	PACKET_INFO,           //��½ʱ�õ�����Ϣ
	PACKET_HEART,          //������
}packet_type;







#endif // _PACKET_TYPE_H
