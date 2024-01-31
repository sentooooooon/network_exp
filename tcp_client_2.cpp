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

#include <arpa/inet.h>
#include <iostream>
#include <unistd.h> // https://linux.die.net/man/2/read
#include <string>
#include<string.h>
#include<chrono>
#include <ctime>
#include <cmath>
#include<random>
const int BUFF_SIZE = 64; // バッファのサイズ

int main(int argc, char* argv[])
{
    using namespace std;
    using namespace chrono;
    cout << "tcp time client v1.1.0" << endl; // ソースコードへの変更を行ったら数値を変える．

    // サーバのアドレスとポート番号
    // 127.0.0.1は，ループバックアドレス
    // 他のPCと通信する場合は，当該PCのIPアドレスに変更する．
    string serv_ip = "127.0.0.1";
    in_port_t serv_port = 5000;
    
    if(argc > 1)
    {
	    serv_ip = argv[1];
    }
    char buff[BUFF_SIZE];// 受信用バッファ
    int n = 0; // 戻り値の保存用に使う変数．

    // ソケット作成，入力はIP，ストリーム型，TCPを指定．
    int socketd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketd < 0) {
        cout << "Failed to createa socket\n";
        return -1;
    }
    // サーバのアドレス等を初期化．
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(serv_ip.c_str());
    serv_addr.sin_port = htons(serv_port);

    

    // サーバに接続する．
    n = connect(socketd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (n < 0) {
        cout << "Failed to connect to the server\n";
        return -1;
    }
    //ランダムにメッセージを生成

    random_device rd;
    mt19937_64 mt(rd());
    uniform_int_distribution<char> cU(30, 80);
    int power = pow(2,16);
    string msg;
    for(int i = 0; i < power; i++){
	      char c = cU(mt);
	      msg += c;
      
    }
    cout << msg << endl;
    //cout <<"----------------------------"<< endl;


    //string msg = "hello\n";
    int j = 0;
    int c = 0;

    auto start = system_clock::now();

    while(true){
	     if(j == msg.size()){
		     c++;
		     buff[c] = '\0';
		     n = write(socketd, buff, sizeof(buff));
		     memset(buff, 0, sizeof(buff));
		     //cout << "break" << endl;
		     break;
	     }
	     else if(c == BUFF_SIZE - 1){
		     buff[c] == '\0';
		     n = write(socketd, buff, sizeof(buff));
		     c = 0;
		    //cout << "sent" << endl;
		     //cout << buff << endl;
		     memset(buff, 0 ,sizeof(buff));
	     }
	     //cout << j << endl;
	     buff[c] = msg[j]; j++; c++;
     }
     //cout << "end loop" << endl;
     buff[0] = '~';
     buff[1] = 0;

    n = write(socketd, buff, sizeof(buff)); // 文字列の送信．第二引数は記憶域．第３引数は送信するByte数．
    memset(buff, 0, sizeof(buff));
    // 接続すると，サーバは現在時刻を文字列として返信する．
    // read(.)により，データを受信する．
    //n = read(socketd, buff, sizeof(buff)-1);
    if (n < 0) {
        // readの戻り値が負の場合，通信に不具合が生じたことを意味する．
        cout << "failed to read from a socket\n";
        return -1;
    }
    // readの戻り値が 0 の場合，相手が接続を遮断したことを意味する．
    //buff[n] = 0;
    // サーバからの返信された文字列（現在時刻）を表示
    //cout << buff;

    auto end = system_clock::now();
    duration<double> elapsed_seconds = end - start;
    cout << endl;
    cout << "TIME :" << elapsed_seconds.count() << "s";

    // close the socket
    close(socketd);
}
