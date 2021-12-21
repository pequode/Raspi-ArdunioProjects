#include "conf.h"
using namespace std;
// main file used for watering plants
ofstream outfile;
vector<int> Confs{-1,-1,-1,-1,-1,-1,-1,-1,-1,-69,-68};//last is always true second to last is always false
// semaphore acting as a mutex to stop race condition
sem_t Signal1;
//writes data to log file
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
// used to update the config file for the web servers use and the use of the program if restarted
void writeVector(){
  sem_wait(&Signal1);//bad to use same sem
  string writePath;
  if(DEBUG)writePath = "cont_debug.txt";
  else writePath = ConfPath;
  outfile.open(writePath);
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
// get a vector from the config file updates global Confs vector
void ReadTimeingsFile(){
    sem_wait(&Signal1);
    string readPath;
    if(!DEBUG) readPath = ConfPath;
    else readPath = "cont_debug.txt";
    ifstream input_file(readPath);

    while (!input_file.is_open()){
      if(!DEBUG)printOutput("ERR:Conf-file is already open sleeping for 5 seconds");
      else cout<< "file open waiting \n";
      sleep(5);
    }
    int number;
    for (int i =0; i<((int)Confs.size()) && input_file >> number;i++)
    {
        Confs[i] = number;
    }
    input_file.close();
    sem_post(&Signal1);
    if(DEBUG) cout<<"Done Read\n";
}
// get a delay time and wait that long, but check for interupts every second
int waitTime(int idForSleepSeconds,int idForReset){
   int i;
   for (i=0;i<Confs[idForSleepSeconds];i++){
       delay (1000);
       if (i%10 == 0){
          ReadTimeingsFile();
       }
       if (1){
           if((idForSleepSeconds == waterRestartIntervalSec ||
               idForSleepSeconds == fanRestartIntervalSec   ||
               idForSleepSeconds == lightRestartIntervalSec) &&  Confs[idForReset] == 1
             )
            {
             if (DEBUG)cout << "meant to be off but on -- shouldnt be printed without website\n";
             Confs[idForReset] = 1;
             writeVector();
             return 1;
            }
           else if(
             (idForSleepSeconds == waterTimeInd ||
              idForSleepSeconds == fanTimeInd   ||
              idForSleepSeconds == lightTimeInd)
              &&  Confs[idForReset] == 0
              ){
              if (DEBUG) cout << "meant to be on but off -- shouldnt be printed without website\n";
              Confs[idForReset] = 0;
              writeVector();
              return 0;
            }
        }



   }

    if (1){
       if(idForSleepSeconds == waterRestartIntervalSec ||
           idForSleepSeconds == fanRestartIntervalSec   ||
           idForSleepSeconds == lightRestartIntervalSec){
          Confs[idForReset] = 1;
       }else{
          Confs[idForReset] = 0;
       }
       writeVector();
       if (DEBUG)cout<<"l99 ish just ran\n";
    }
   if (DEBUG) cout << "done Reg\n";
   return -1;
}
// threaded function used for watering the plant
void* wateringCycle(void* arg){
   while(true){
      ReadTimeingsFile();

      if (!DEBUG) digitalWrite (Water, HIGH);
      if (!DEBUG) printOutput("Starting Watering: ");
      else cout << "fake Water on\n";

      waitTime(waterTimeInd,waterRestartTimerInd);

      if (!DEBUG) digitalWrite (Water,  LOW);
      if (!DEBUG) printOutput("Water Off: ");
      else cout << "fake Water off\n";

      waitTime(waterRestartIntervalSec,waterRestartTimerInd);

   }
    return NULL;
}
// threaded function used for decreasing humidity by running air from outside
void* fanCycle(void* arg){
 while(true){
    ReadTimeingsFile();

    if (!DEBUG) digitalWrite (Fans, HIGH);
    if (!DEBUG) printOutput("Starting Vent: ");
    else cout << "fake vent on\n";

    waitTime(fanTimeInd,fanRestartTimerInd);

    if (!DEBUG) digitalWrite (Fans,  LOW);
    if (!DEBUG) printOutput("Vent Off: ");
    else cout << "fake vent off\n";

    waitTime(fanRestartIntervalSec,fanRestartTimerInd);
 }
 return NULL;
}
// threaded function used for lighing the plant
void* lightTimer(void* arg){
  while(true){
    ReadTimeingsFile();

    if (!DEBUG)digitalWrite (Lights,  HIGH);
    if (!DEBUG)printOutput("Lights On: ");
    else cout << "fake light on\n";

    waitTime(lightRestartIntervalSec,lightRestartTimerInd);

    if (!DEBUG)digitalWrite (Lights, LOW);
    if (!DEBUG)printOutput("Lights Off: ");
    else cout << "fake light on\n";

    waitTime(lightTimeInd,lightRestartTimerInd);

  }
  return NULL;
}

// checks for a kill signal and lets it fail safe for the RaspberryPi
void signal_callback_handler(int signum) {
   if(DEBUG)cout << "Signal Caught" << signum << endl;

   sem_destroy(&Signal1);
   if (!DEBUG)digitalWrite (Lights,  LOW);
   if (!DEBUG)digitalWrite (Water,  LOW);
   if (!DEBUG)digitalWrite (Fans,  LOW);
   if(DEBUG)cout << "Killed Program\n";
   exit(signum);
}


int main (void)
{
  // initialize the pins
  if (!DEBUG){
    wiringPiSetup();
    pinMode (Water, OUTPUT) ;
    pinMode (Fans, OUTPUT) ;
    pinMode (Lights, OUTPUT) ;

    digitalWrite (Lights,  LOW);
    digitalWrite (Water,  LOW);
    digitalWrite (Fans,  LOW);
  }
  //create the signal signal handler
  signal(SIGINT, signal_callback_handler);
  // startup delay to allow web server to boot
  if (!DEBUG)delay(10000);
  else cout << "skipping line \n";
  if (!DEBUG)digitalWrite (Lights,  HIGH);
  // initialize sem
  sem_init(&Signal1, 0, 1);
  time_t now = time(0);
  // each function acts as a thread
  if(DEBUG)cout << "  Example seconds is : "<< now <<endl;
  pthread_t threads[3];
  pthread_create(&threads[0],NULL,wateringCycle,NULL);
  pthread_create(&threads[1],NULL,fanCycle,NULL);
  pthread_create(&threads[2],NULL,lightTimer,NULL);
  // done so main isnt run
  pthread_join(threads[0],NULL);
  pthread_join(threads[1],NULL);
  pthread_join(threads[2],NULL);

  sem_destroy(&Signal1);

  if (!DEBUG)digitalWrite (Lights,  LOW);
  if (!DEBUG)digitalWrite (Water,  LOW);
  if (!DEBUG)digitalWrite (Fans,  LOW);
  // sig sev should stop this from being reached but it still fails safe
  if(DEBUG)cout << "Killed Program never should happen\n";

  return 0 ;
}
