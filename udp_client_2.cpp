//
// 情報通信応用実験 ネットワークプログラミング
//
// 首都大学東京 システムデザイン学部 情報通信システムコース
// 准教授・酒井和哉
// ２０１５年２月５日
//
// 情報科学科
// 助教・柴田祐樹
// ２０１９年１０月　改訂
// ２０２０年１０月　改訂
//
#include<string.h>
#include <arpa/inet.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include<random>
#include<cmath>
#include <unistd.h> // https://linux.die.net/man/2/read
const int BUFF_SIZE = 64; // バッファのサイズ


/*
 * UDP Daytimeクライアント
 */
int main(int argc, char* argv[])
{
    using namespace std;
    using namespace chrono;
    cout << "upd time client v1.1.0" << endl; // ソースコードへの変更を行ったら数値を変える．
    string serv_ip = "127.0.0.1"; // ループバックアドレス
    in_port_t port_num = 5000; // ポート番号
    int n = 0; // 戻り値の保存用
    char buff[BUFF_SIZE]; // 送受信用バッファ
    //
    

    if (argc == 2) {
        serv_ip = argv[1];
    }
    // パラメータの初期化
    struct sockaddr_in serv_addr; // アドレス構造体
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(serv_ip.c_str());
    serv_addr.sin_port = htons(port_num);

    // ソケットの作成．UDPを用いるため第２引数にDatagram，第３引数にUDPを指定する．
    int socketd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketd < 0) {
        cout << "Failed to create a client socket.\n";
        return -1;
    }
	
    //set random
    random_device rd;
    mt19937_64 mt(rd());
    uniform_int_distribution<char> cU(30, 80);
    
    int power = pow(2,25);
    int count = 0;
    string  msg;
      
     //generate random message
    for(int i = 0; i < power; i++){
	      char c = cU(mt);
	      msg += c;
      }
    //cout << "made random message" << endl;
    cout << msg << endl;
    //cout << "------------------------------------------------" << endl;
	int c = 0; 
	int j = 0;
    auto start = system_clock::now();
	
     while(true){
	     if(j == msg.size()){
		     c++;
		     buff[c] = 0;
		     n = sendto(socketd, buff, sizeof(buff), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		     memset(buff, 0, sizeof(buff));
		     //cout << "break" << endl;
		     break;
	     }
	     else if(c == BUFF_SIZE - 1){
		     buff[c] = 0;
		     n = sendto(socketd, buff, sizeof(buff), 0 , (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		     c = 0;
		     //cout << "sent" << endl;
		     //cout << buff << endl;
		     memset(buff, 0 ,sizeof(buff));
	     }
	     //cout << j << endl;
	     buff[c] = msg[j]; j++; c++;
     }
     buff[0] = '~';
     buff[1] = 0;
     n = sendto(socketd, buff, sizeof(buff), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    //cout << "loop deta" << endl;

    //if(msg == "END")break;
    //n = sendto(socketd, buff, BUFF_SIZE, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (n < 0) {
        cout << "failed to receive a message.\n";
        return -1;
    }
    // サーバから現在時刻を文字列として受信．
    //cout << "befor recieve" << endl;
    //n = recvfrom(socketd, buff, sizeof(buff)-1, 0, NULL, NULL); // 終端文字列を入れるために，sizeof(buff)-1 として，文字列一つ分必ず余裕を持たせてデータを受信する．buff をこのまま文字列として使わない場合は全記憶を受信に使う．
    if (n < 0) {
        cout << "Failed to receive a message.\n";
        return -1;
    }
    //cout << "after recieve" << endl;
    auto end = system_clock::now();

    duration<double> elapsed_seconds = end - start;

    buff[n] = 0; // 終端文字列を追加．送信者が終端文字列を入れてデータを送ってきているとは限らない．
    cout << "Time: " << elapsed_seconds.count() << "s" << "\n";
    memset(buff, 0, sizeof(buff));
	//cout << "end" <<endl;
    // ソケットを閉じる
    close(socketd);
}