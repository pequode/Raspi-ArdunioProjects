#include "conf.h"
using namespace std;
ofstream outfile;
vector<int> Confs{-1,-1,-1,-1,-1,-1,-1,-1,-1,-69,-68};//last is always true second to last is always false

sem_t Signal1;

void printOutput(string K){
  sem_wait(&Signal1);//bad to use same sem
  outfile.open(LogPath,ios_base::app);
  time_t now = time(0);
  tm *ltm = localtime(&now);
  outfile << K << (1900 + ltm->tm_year)<<":"<<( 1 + ltm->tm_mon)<<":"<<(ltm->tm_mday)<<":"<<( 5+ltm->tm_hour)<<":"<<(30+ltm->tm_min)<<":"<<(ltm->tm_sec)<<"\n";
  if(DEBUG) cout << K << "\n";
  outfile.close();
  sem_post(&Signal1);//bad to use same sem
}
void writeVector(){
  sem_wait(&Signal1);//bad to use same sem
  outfile.open("testFile.txt");
  for(int i =0 ; i<int(Confs.size());i++){
    outfile << Confs[i] << "\n";
  }
  if(DEBUG){
    for(int i =0 ; i<int(Confs.size());i++){
      cout << Confs[i] << "\n";
    }
  }
  outfile.close();
  sem_post(&Signal1);//bad to use same sem
}
void ReadTimeingsFile(){
    sem_wait(&Signal1);
    ifstream input_file(ConfPath);
    while (!input_file.is_open()){
      printOutput("ERR:Conf-file is already open sleeping for 5 seconds");
      sleep(5);
    }
    int number;
    for (int i =0; i<((int)Confs.size()) && input_file >> number;i++)
    {
        Confs[i] = number;
    }
    input_file.close();
    sem_post(&Signal1);
    if(DEBUG) cout<<"Done Read\n\n";
}

int waitTime(int idForSleepSeconds,int idForReset){
   int i;
   //int Reset = 0;
   for (i=0;i<Confs[idForSleepSeconds];i++){
       delay (1000);
       if (i%10 == 0){
          ReadTimeingsFile();
       }
 /*
       if((idForSleepSeconds == waterRestartIntervalSec ||
           idForSleepSeconds == fanRestartIntervalSec   ||
           idForSleepSeconds == lightRestartIntervalSec) &&  Confs[idForReset] == 1
         )
        {
         cout << "meant to be off but on\n";
         Confs[idForReset] = 1;
         writeVector();
         return 1;
        }
       else if(
         (idForSleepSeconds == waterTimeInd ||
          idForSleepSeconds == fanTimeInd   ||
          idForSleepSeconds == lightTimeInd) &&  Confs[idForReset] == 0
        ){
          cout << "meant to be on but off\n";
          Confs[idForReset] = 0;
          writeVector();
          return 0;
        }
*/


   }
  /
  /* if(idForSleepSeconds == waterRestartIntervalSec ||
       idForSleepSeconds == fanRestartIntervalSec   ||
       idForSleepSeconds == lightRestartIntervalSec){
      Confs[idForReset] = 1;
   }else{
      Confs[idForReset] = 0;
   }
   writeVector*/
   cout << "done Reg\n";
   return -1;
}
void* wateringCycle(void* arg){
   while(true){
    ReadTimeingsFile();

    digitalWrite (Water, HIGH);
    printOutput("Starting Watering: ");
    waitTime(waterTimeInd,waterRestartTimerInd);

    digitalWrite (Water,  LOW);
    printOutput("Water Off: ");
    waitTime(waterRestartIntervalSec,waterRestartTimerInd);

   }
    return NULL;
}
void* fanCycle(void* arg){
 while(true){
  ReadTimeingsFile();

  digitalWrite (Fans, HIGH);
  printOutput("Starting Vent: ");
  waitTime(fanTimeInd,fanRestartTimerInd);

  digitalWrite (Fans,  LOW);
  printOutput("Vent Off: ");
  waitTime(fanRestartIntervalSec,fanRestartTimerInd);
 }
 return NULL;
}
void* lightTimer(void* arg){
  while(true){
    ReadTimeingsFile();

    digitalWrite (Lights,  HIGH);
    printOutput("Lights On: ");
    waitTime(lightRestartIntervalSec,lightRestartTimerInd);

    digitalWrite (Lights, LOW);
    printOutput("Lights Off: ");
    waitTime(lightTimeInd,lightRestartTimerInd);

  }
  return NULL;
}

void signal_callback_handler(int signum) {
   if(DEBUG)cout << "Signal Caught" << signum << endl;

   sem_destroy(&Signal1);
   digitalWrite (Lights,  LOW);
   digitalWrite (Water,  LOW);
   digitalWrite (Fans,  LOW);
   if(DEBUG)cout << "Killed Program\n";
   exit(signum);
}

int main (void)
{
  wiringPiSetup();
  pinMode (Water, OUTPUT) ;
  pinMode (Fans, OUTPUT) ;
  pinMode (Lights, OUTPUT) ;

  digitalWrite (Lights,  LOW);
  digitalWrite (Water,  LOW);
  digitalWrite (Fans,  LOW);
  signal(SIGINT, signal_callback_handler);
  delay(10000);
  digitalWrite (Lights,  HIGH);

  sem_init(&Signal1, 0, 1);
  time_t now = time(0);
  if(DEBUG)cout << "  Example seconds is : "<< now <<endl;
  pthread_t threads[3];
  pthread_create(&threads[0],NULL,wateringCycle,NULL);
  pthread_create(&threads[1],NULL,fanCycle,NULL);
  pthread_create(&threads[2],NULL,lightTimer,NULL);

  pthread_join(threads[0],NULL);
  pthread_join(threads[1],NULL);
  pthread_join(threads[2],NULL);

  sem_destroy(&Signal1);

  digitalWrite (Lights,  LOW);
  digitalWrite (Water,  LOW);
  digitalWrite (Fans,  LOW);

  if(DEBUG)cout << "Killed Program\n";

  return 0 ;
}
