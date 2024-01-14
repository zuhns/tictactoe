/*Tris Game*/
#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>
#include <string>

using namespace std;


bool GetLoopForGame = true;

char TavolaGioco[3][3]
{
  {'0','1','2'},
  {'3','4','5'},
  {'6','7','8'},
};

bool Posizione [9] = {false,false,false,false,false,false,false,false,false};
bool MoveAIVector[9]={false, false, false, false, false, false, false, false, false};
bool MovePlayerVector[9]={false, false, false, false, false, false, false, false, false};
bool bb = true;
bool loopen = true;
int draw = 0;
int checks = 0;

void GetMenu(char InputModality);
void NewGame();
void Credits();

//Funzione che ha il compito solo della stampa della tabella
void CoutTable()
{
  for(int i = 0; i < 3; i++)
    {
      for(int y = 0; y < 3; y++)
        {
          if(TavolaGioco[i][y]=='X')
          {
            cout<<"\033[31m"<<TavolaGioco[i][y]<<"\033[0m ";
          }
          else if(TavolaGioco[i][y]=='O')
          {
            cout<<"\033[34m"<<TavolaGioco[i][y]<<"\033[0m ";
          }
          else
          {
           cout<<TavolaGioco[i][y]<<" ";
          }
        }
      cout<<endl;
    }
  cout<<endl;
}

//Funzione che ha il compito di cambiare i numeri con 'X' o 'O'
void ChangeTable(int i, int y, bool z)
{
  draw++;
  if(z)
    TavolaGioco[i][y]='X';
  else
    TavolaGioco[i][y]='O';
  CoutTable();
}
//Funzione che ha il compito di vedere le mosse disponibili solo per il/i giocatore/i
bool FreePosition(int a)
{
  if(Posizione[a])
  {
    cout<<"\nCasella già occupata, si prega di riscegliere\n\n";
    return false;
  }
  Posizione[a] = true;
  return true;
}
//Funzione che ha il compito di trasformare l'input del utente in numero dato per eseguire la sua mossa in indici per la tabella
void ChangeNumberToIndex(int a, bool b)
{
  int i=0;
  int y=0;
  if(a >= 0 && a < 9)
  {
    if(a<3 && a>=0)
    {
      i=0;
      y=a;
    }
    else if(a<6 && a>=3)
    {
      i=1;
      y=a-3;
    }
    else if(a<9 && a>=6)
    {
      i=2;
      y=a-6;
    }
    ChangeTable(i, y, b);
  }
}
//Funzione che ha il compito di ripristinare le variabili globali a fine di ogni partita
void ResetGame()
{
  // Reimposta le variabili di gioco
  draw = 0;
  checks = 0;
  bb = true;
  for (int i = 0; i < 9; i++)
  {
    Posizione[i] = false;
    MoveAIVector[i]= false;
    MovePlayerVector[i] = false;
  }

  // Reimposta la tavola di gioco
  TavolaGioco[0][0] = '0';
  TavolaGioco[0][1] = '1';
  TavolaGioco[0][2] = '2';
  TavolaGioco[1][0] = '3';
  TavolaGioco[1][1] = '4';
  TavolaGioco[1][2] = '5';
  TavolaGioco[2][0] = '6';
  TavolaGioco[2][1] = '7';
  TavolaGioco[2][2] = '8';
}
//Funzione che ha il compito di verificare l'input giusto dall'utente
bool ValidArgument(int a)
{
  if(a<0 || a>8)
  {
    return true;
  }
  return false;
}
//Funzione che ha il compito di verificare vittorie o pareggi.
bool CheckWin()
{
  int i=0;
  int y=0;
  while(i<3)
    {
      if(TavolaGioco[i][y] == TavolaGioco[i][y+1] && TavolaGioco[i][y] == TavolaGioco[i][y+2])
      {
        cout<<"\n\n\n\033[41m'"<<TavolaGioco[i][y]<<"'\033[0m\033[31m" << " ha vinto la partita\033[0m (tris orizzonale)\n\n\n";
        ResetGame();
        return true;
      }
      i++;
    }
  i = 0;
  while(y<3)
    {
      if(TavolaGioco[i][y] == TavolaGioco[i+1][y] && TavolaGioco[i][y] == TavolaGioco[i+2][y])
      {
        cout<<"\n\n\n\033[42m'"<<TavolaGioco[i][y]<<"'\033[0m\033[32m" << " ha vinto la partita\033[0m (tris verticale)\n\n\n";
        ResetGame();
        return true;
      }
      y++;
    }
  y = 0;
  if(TavolaGioco[i][y] == TavolaGioco[i+1][y+1] && TavolaGioco[i][y] == TavolaGioco[i+2][y+2])
  {
    cout<<"\n\n\n\033[46m'"<<TavolaGioco[i][y]<<"'\033[0m\033[36m" << " ha vinto la partita\033[0m (tris obliquo)\n\n\n";
    ResetGame();
    return true;
  }
  else if (TavolaGioco[0][y+2] == TavolaGioco[i+1][y+1] && TavolaGioco[0][y+2] == TavolaGioco[i+2][y])
  {
    cout<<"\n\n\n\033[46m'"<<TavolaGioco[0][y+2]<<"'\033[0m\033[36m" << " ha vinto la partita\033[0m (tris obliquo)\n\n\n";
    ResetGame();
    return true;
  }
  
  if (draw == 9)
  {
    cout << "\n\n\n\033[47mLa Partita si è conclusa con il Pareggio\033[0m🤝\n\n\n";
    ResetGame();
    return true;
  }
  return false;
}
//Funzione principale che si occupa della gestione nelle partite 1vs1
void PlayGameCoop()
{
  CoutTable();
  bool xy = true;
  while(true)
    {
      int InputPlayer1;
      int InputPlayer2;

      while(true)
      {
        cout<<"Turno Giocatore 1 = 'X'"<<endl;
        if(!(cin>>InputPlayer1))
        {
          cout<<"\n\n\nValori non validi, si prega di scegliere un numero intero compreso da 0 a 8\n\n\n";
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
          continue;
        }
        if(ValidArgument(InputPlayer1))
        {
          cout<<"\n\n\nValori non validi, si prega di scegliere un numero da 0 a 8\n\n\n";
          continue;
        }
        if(FreePosition(InputPlayer1))
          break;
      }
      xy = true;
      ChangeNumberToIndex(InputPlayer1, xy);
      if(CheckWin())
        break;

      while(true)
      {
        cout<<"Turno Giocatore 2 = 'O'"<<endl;
        if(!(cin>>InputPlayer2))
        {
          cout<<"\n\n\nValori non validi, si prega di scegliere un numero intero compreso da 0 a 8\n\n\n";
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
          continue;
        }
        if(ValidArgument(InputPlayer2))
        {
          cout<<"\n\n\nValori non validi, si prega di scegliere un numero da 0 a 8\n\n\n";
          continue;
        }
        if(FreePosition(InputPlayer2))
          break;
      }
      xy = false;
      ChangeNumberToIndex(InputPlayer2, xy);
      if(CheckWin())
        break;
    }
}
//Generatore di mosse causali AI
int AlgoritmoCasualeAI(int Valorecasuale)
{
  return rand()%Valorecasuale;    
}

//Funzione che si occupa di ritornare il valore casuale
int FindRandomMove()
{
  int RandomMove = AlgoritmoCasualeAI(9);
  return RandomMove;
}

//Funzione che verifica le disposizioni disponibili
  bool IsFreeForAI(int a)
  {
    if(Posizione[a])
    {
      return false;
    }
    Posizione[a] = true;
    return true;
  }

//Funzione principale che si occupa di gestire il gioco tra utente e AI
void PlayGameRandomAI()
{
  CoutTable();
  bool xy = true;//variabile bool che determina il turno del Giocatore o dell'AI
  while(true)//ciclo che determina la fine della partita
    {
      int InputPlayer1;//variabile principale per la gestione del input del player per decidere la sua mossa
      int InputMoveAI;//variabile principale per la gestione del input dell'AI per decidere la sua mossa

      while(true)//ciclo che serve in caso la posizione data dal player sia occupata
      {
        cout<<"Turno Giocatore 1 = 'X'"<<endl;
        if(!(cin>>InputPlayer1))
        {
          cout<<"\n\n\nValori non validi, si prega di scegliere un numero intero compreso da 0 a 8\n\n\n";
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
          continue;
        }
        if(ValidArgument(InputPlayer1))//l'input del giocatore è compreso tra 0 e 8?
        {
          cout<<"\n\n\nValori non validi, si prega di scegliere un numero da 0 a 8\n\n\n";
          continue;//richiedi in input
        }
        if(FreePosition(InputPlayer1))
          break;//Posizione libera quindi esce dal ciclo
      }
      xy = true;//turno del Giocatore
      ChangeNumberToIndex(InputPlayer1, xy);//modifica l'input in indici
      if(CheckWin())//controlla se ci sono pareggi o vittorie dopo la mossa
        break;

      cout<<"Turno Dell'AI = 'O'"<<endl;
      while(true)
      {
        InputMoveAI = FindRandomMove();
        if(IsFreeForAI(InputMoveAI))//verfica per l'AI le mosse libere
          break;
      }
      xy = false;//turno dell'AI
      ChangeNumberToIndex(InputMoveAI, xy);
      if(CheckWin())
        break;
    }
}

int GetOpposite(int n)// funzione che ritorna i rispettivi valori contrari nella tavola
{
    if(n==0)
        return 8;
    else if(n==1)
        return 7;
    else if(n==2)
        return 6;
    else if(n==3)
        return 5;
    else if(n==5)
        return 3;
    else if(n==6)
        return 2;
    else if(n==7)
        return 1;
    else if(n==8)
        return 0;
    return 0;
}

int GetParallel(int n)// funzione che ritorna i rispettivi valori secondo uno schema opposto verso y
{
  if(n==0)
    return 6;
  else if(n==2)
    return 8;
  else if(n==6)
    return 0;
  else if(n==8)
    return 2;
  else
    return -1;
}

int GetAntiParallel(int n)// funzione che ritorna i rispettivi valori secondo uno schema opposto verso x
{
  if(n==0)
    return 2;
  else if(n==6)
    return 8;
  else if(n==8)
    return 6;
  else if(n==2)
    return 0;
  else
    return -1;
}

int GetL(int a, int b) // funzione che ritorna i rispettivi valori secondo uno schema ad L
{
  if(a == 0 && b == 7 || a == 2 && b == 7)
    return GetParallel(a);
  else if(a == 0 && b == 5 || a == 6 && b == 5)
    return GetAntiParallel(a);
  else if(a == 2 && b == 3 || a == 8 && b == 3)
    return GetAntiParallel(a);
  else if(a == 8 && b == 1 || a == 6 && b == 1)
    return GetParallel(a);
  else return -1;
}

bool CheckAIWin() //funzione che determina casi dove l AI vince
{
    int i =0;
    int y=0;
    while(y<3)
    {
        y++;
        if(MoveAIVector[0+i] == MoveAIVector[1+i] && MoveAIVector[1+i] == MoveAIVector[2+i] && MoveAIVector[0+i]== MoveAIVector[2+i] && MoveAIVector[0+i])
            return true;
        i= i+3;
    }
    i =0;
    while (i<3)
    {
        if(MoveAIVector[0+i] == MoveAIVector[3+i] && MoveAIVector[0+i] == MoveAIVector[6+i] && MoveAIVector[3+i] == MoveAIVector[6+i] && MoveAIVector[0+i])
            return true;
        i++;
    }
    if(MoveAIVector[2] == MoveAIVector[4] && MoveAIVector[4] ==  MoveAIVector[6] && MoveAIVector[2] == MoveAIVector[6] && MoveAIVector[2] && MoveAIVector[4] || MoveAIVector[0] == MoveAIVector[4] && MoveAIVector[4] ==  MoveAIVector[8] && MoveAIVector[0] == MoveAIVector[8] && MoveAIVector[0] && MoveAIVector[4])
        return true;
    return false;
}


int FindPossibleTris()// funzione che cerca possibili tris per l'AI
{
    for(int i = 0; i<9; i++)
    {
        if(MoveAIVector[i] == false)
        {
            MoveAIVector[i] = true;//assegna un valore temporaneo per verficare condizioni di tris
        }
        else
        {
          continue;
        }
        if(CheckAIWin() && MoveAIVector[i] != MovePlayerVector[i])
        {
            return i;// posizone dove l'AI vince
        }
        else
        {
          MoveAIVector[i] = false;// valore temporaneo rimosso
        }
    }
    return -1;// nessun tris disponibile per l AI
}

bool CheckPlayerWin()// funzione che determina mosse vincenti da parte dell'utente, funzione creata solo per l'obbiettivo di fornite dati alle funzioni blockEnemy() e predictEnemyDoubleMove()
{
  int i =0;
  int y=0;
  while(y<3)
  {
      y++;
      if(MovePlayerVector[0+i] == MovePlayerVector[1+i] && MovePlayerVector[1+i] == MovePlayerVector[2+i] && MovePlayerVector[0+i]== MovePlayerVector[2+i] && MovePlayerVector[0+i])
         {
          checks ++;
          return true;
         } 
      i= i+3;
  }
  i =0;
  while (i<3)
  {
      if(MovePlayerVector[0+i] == MovePlayerVector[3+i] && MovePlayerVector[0+i] == MovePlayerVector[6+i] && MovePlayerVector[3+i] == MovePlayerVector[6+i] && MovePlayerVector[0+i])
        {
          checks ++;
          return true;
        } 
      i++;
  }
  if(MovePlayerVector[2] == MovePlayerVector[4] && MovePlayerVector[4] ==  MovePlayerVector[6] && MovePlayerVector[2] == MovePlayerVector[6] && MovePlayerVector[2] && MovePlayerVector[4] || MovePlayerVector[0] == MovePlayerVector[4] && MovePlayerVector[4] ==  MovePlayerVector[8] && MovePlayerVector[0] == MovePlayerVector[8] && MovePlayerVector[0] && MovePlayerVector[4])
    {
      checks ++;
      return true;
    } 
  return false;
}

int blockEnemy() // funzione che ha il compito di bloccare le mosse nemiche
{
  for(int i = 0; i<9; i++)
    {
        if(MovePlayerVector[i] == false)
        {
            MovePlayerVector[i] = true;
        }
        else
        {
          continue;
        }
        if(CheckPlayerWin() && MovePlayerVector[i] != MoveAIVector[i])
        {
            MovePlayerVector[i] = false;
            return i;
        }
        else
        {
          MovePlayerVector[i] = false;
        }
    }
    return -1;
}

int predictEnemyDoubleMove() // funzione che cerca di predirre le doppie mosse che l'utente a intenzione di fare(senza questa funzione possiamo fare la modalità medium)
{
  checks = 0;
  bool beforeY= false;
  bool beforeI = false;
  bool check = false;
  int index = 0;

  for(int i = 0; i < 9; i++)
    {
      beforeI = MovePlayerVector[i];
      MovePlayerVector[i] = true; //assegnazioni posizione false temporanee
      if(MovePlayerVector[i] != MoveAIVector[i])
      {
        for(int y=i+1; y< 9; y++)
        {
          beforeY = MovePlayerVector[y];
          MovePlayerVector[y] = true;//assegnazioni posizione false temporanee
          if(CheckPlayerWin() && MovePlayerVector[y] != MoveAIVector[y] && checks>=2)
          {
              index = i; // valore per bloccare la possibile doppia mossa dell utente
              check =  true; // variabile globale
          }
          MovePlayerVector[y] = beforeY;//rimozione posizione false temporanee
        }
      }
      MovePlayerVector[i] = beforeI;//rimozione posizione false temporanee
    }
  if(check)
    return index;
  return -1; // possibile doppia mossa non trovata
}

//Funzione con AI ad albero decisionale, con tentativi nella sua memoria per provare dei probabili tris (piccola AI)
int InputPlayerMenager()
{
    int InputPlayer1;//variabile principale per la gestione del input del player per decidere la sua mossa

    while(true)//ciclo che serve in caso la posizione data dal player sia occupata
    {
    cout<<"Turno Giocatore 1 = 'X'"<<endl;
    if(!(cin>>InputPlayer1)) //tentativo di leggere un valore da cin e assegnarlo alla variabile InputPlayer1
    {
      cout<<"\n\n\nValori non validi, si prega di scegliere un numero intero compreso da 0 a 8\n\n\n";
      cin.clear(); //ripristinare lo stato del flusso cin in modo che sia pronto a leggere nuovi dati.
      cin.ignore(numeric_limits<streamsize>::max(), '\n'); //viene utilizzato per ignorare tutti i caratteri rimanenti nella linea corrente
      continue;
    }
    if(ValidArgument(InputPlayer1))//l'input del giocatore è compreso tra 0 e 8?
    {
        cout<<"\n\n\nValori non validi, si prega di scegliere un numero da 0 a 8\n\n\n";
        continue;//richiedi in input
    }
    if(FreePosition(InputPlayer1))
        break;//Posizione libera quindi esce dal ciclo
    }
    bb = true;//turno del Giocatore
    MovePlayerVector[InputPlayer1]=true;
    ChangeNumberToIndex(InputPlayer1, bb);//modifica l'input in indici
    if(CheckWin())
        loopen = false;

    return InputPlayer1;
}

void AIMenager(int InputMoveAI) //funzione che si occupa del Input del AI
{
    cout<<"Turno Dell'AI = 'O'"<<endl;

    MoveAIVector[InputMoveAI] = true; //assegazione della posisizione solo dell AI
    Posizione[InputMoveAI] = true; // assegnazone della posizione dell AI nel vettore totale insieme al utente
    bb = false;//turno dell'AI
    ChangeNumberToIndex(InputMoveAI, bb); //cambio per la tabella
    if(CheckWin()) //se vince
        loopen = false; //finisce il ciclo per uscire o determina se prendere in input la parte dell utente
}

void PlayGameImpossibleMode()
{
    CoutTable();
    
    int Possibletris = 0; //per un solo richiamo della funzione FindPossibleTris
    int BlockEMove = 0; //per un solo richiamo della funzione blockEnemey

    while(loopen)
    {
        int InputMoveAI;//variabile principale per la gestione del input dell'AI per decidere la sua mossa
        int InputPlayer;

        InputPlayer = InputPlayerMenager();
        int temp = InputPlayer;
        
        if(Posizione[4])//blocco codice se l'utente inizia dal centro
        {
            InputMoveAI = 2;
            AIMenager(InputMoveAI);

            InputPlayer = InputPlayerMenager();

            if(Posizione[GetOpposite(InputPlayer)])
            {
                InputMoveAI = GetParallel(InputMoveAI);
                AIMenager(InputMoveAI);

                InputPlayer = InputPlayerMenager();

                Possibletris = FindPossibleTris();
                if(Possibletris >= 0)
                {
                  InputMoveAI = Possibletris;
                  AIMenager(InputMoveAI); //AI Win
                }
                else
                {
                  BlockEMove = blockEnemy();
                  if(BlockEMove >= 0)
                  {
                      InputMoveAI = BlockEMove;
                      AIMenager(InputMoveAI);

                      InputPlayer = InputPlayerMenager();

                      BlockEMove = blockEnemy();
                      if(BlockEMove >= 0)
                      {
                        InputMoveAI = BlockEMove;
                        AIMenager(InputMoveAI);

                        InputPlayer = InputPlayerMenager();//Pareggio
                      }
                      else
                      {
                        while(true)
                          {
                              InputMoveAI = FindRandomMove();
                              if(!Posizione[InputMoveAI])//verfica per l'AI le mosse libere
                                  break;
                          }
                      }
                  }
                  else
                  {
                      InputMoveAI = FindPossibleTris();
                      AIMenager(InputMoveAI); //AI Win
                  }
                }
            }
            else if(!Posizione[6])
            {
                InputMoveAI = GetOpposite(InputPlayer);
                AIMenager(InputMoveAI);

                InputPlayer = InputPlayerMenager();

                Possibletris = FindPossibleTris();
                if(Possibletris >= 0)
                {
                    InputMoveAI = Possibletris;
                    AIMenager(InputMoveAI); //AI Win
                }
                else
                {
                    if(Posizione[6])
                    {
                      InputMoveAI = 0;
                    }
                    else
                    {
                      InputMoveAI = GetOpposite(InputPlayer);
                    }
                    AIMenager(InputMoveAI);

                    if(loopen)
                      InputPlayer = InputPlayerMenager();
                    if(Posizione[6])
                    {
                        Possibletris = FindPossibleTris();
                        if(Possibletris >= 0)
                        {
                            InputMoveAI = Possibletris;
                            AIMenager(InputMoveAI); //AI Win
                        }
                        else
                        {
                          if(Posizione[GetOpposite(InputPlayer)] != Posizione[InputPlayer])
                          {
                            InputMoveAI = GetOpposite(InputPlayer);
                            AIMenager(InputMoveAI);
                            if(loopen)
                              InputPlayer = InputPlayerMenager();
                          }
                          if(loopen)
                          {
                            while(true)
                            {
                                InputMoveAI = FindRandomMove();
                                if(!Posizione[InputMoveAI])//verfica per l'AI le mosse libere
                                  break;
                            }
                            AIMenager(InputMoveAI);

                            if(loopen)
                              InputPlayerMenager(); // Pareggio
                          }
                        }
                    }
                    else
                    {
                        Possibletris = FindPossibleTris();
                        if(Possibletris >= 0)
                        {
                            InputMoveAI = Possibletris;
                            AIMenager(InputMoveAI); //AI Win
                        }
                        else
                        {
                          InputMoveAI = GetOpposite(InputPlayer);
                          AIMenager(InputMoveAI);

                          InputPlayer = InputPlayerMenager();//Pareggio
                        }
                    }
                }
            }
        }


        else if(!Posizione[4])//blocco codice se l'utente inizia non dal centro
        {
          InputMoveAI = 4;
          AIMenager(InputMoveAI);

          InputPlayer = InputPlayerMenager();
          if(Posizione[GetOpposite(InputPlayer)] == Posizione[InputPlayer])
          {
            if(Posizione[0] || Posizione[2] || Posizione[6] || Posizione[8])
            {
              InputMoveAI = GetOpposite(InputPlayer);
              if(InputMoveAI!= 8)
              {
                InputMoveAI++;
              }
              else
              {
                InputMoveAI--;
              }
              AIMenager(InputMoveAI);

              InputPlayer = InputPlayerMenager();

              Possibletris = FindPossibleTris();
              if(Possibletris >= 0)
              {
                  InputMoveAI = Possibletris;
                  AIMenager(InputMoveAI); //AI Win
              }
              else
              {
                BlockEMove = blockEnemy();
                if(BlockEMove >= 0)
                {
                  InputMoveAI = BlockEMove;
                  AIMenager(InputMoveAI);

                  InputPlayer = InputPlayerMenager();

                  Possibletris = FindPossibleTris();
                  if(Possibletris >= 0)
                  {
                      InputMoveAI = Possibletris;
                      AIMenager(InputMoveAI); //AI Win
                  }
                  else
                  {
                    BlockEMove = blockEnemy();//dopo questo prof, vorrei un 10 in pagella
                    if(BlockEMove >= 0)
                    {
                      InputMoveAI = BlockEMove;
                      AIMenager(InputMoveAI);

                      if(loopen)
                        InputPlayer = InputPlayerMenager(); //pareggio
                    }
                  }
                }
              }
            }
            else
            {
              if(GetOpposite(InputPlayer) > InputPlayer)
              {
                InputMoveAI = GetOpposite(InputPlayer) - 1;
              }
              else
              {
                 InputMoveAI = InputPlayer - 1;
              }
              InputMoveAI = BlockEMove;
              AIMenager(InputMoveAI);

              InputPlayer = InputPlayerMenager();

              Possibletris = FindPossibleTris();
              if(Possibletris >= 0)
              {
                  InputMoveAI = Possibletris;
                  AIMenager(InputMoveAI); //AI Win
              }
              else
              {
                BlockEMove = blockEnemy();
                if(BlockEMove >= 0)
                {
                  InputMoveAI = BlockEMove;
                  AIMenager(InputMoveAI);

                  if(loopen)
                    InputPlayer = InputPlayerMenager();
                  
                  Possibletris = FindPossibleTris(); //doppia mossa AI Win
                  if(Possibletris >= 0)
                  {
                      InputMoveAI = Possibletris;
                      AIMenager(InputMoveAI); //AI Win
                  }
                }
              }
            }
          }
          else
          {
            BlockEMove = blockEnemy();
            if(BlockEMove >= 0)
            {
              InputMoveAI = BlockEMove;
              AIMenager(InputMoveAI);

              InputPlayer = InputPlayerMenager();

              while(loopen)
              {
                Possibletris = FindPossibleTris();
                if(Possibletris >= 0)
                {
                    InputMoveAI = Possibletris;
                    AIMenager(InputMoveAI); //AI Win
                }
                else
                {
                  BlockEMove = blockEnemy();

                  if(BlockEMove >= 0)
                  {
                    InputMoveAI = BlockEMove;
                    AIMenager(InputMoveAI);

                    if(loopen)
                      InputPlayer = InputPlayerMenager(); 
                  }
                  else
                  {
                    while(true)
                    {
                        InputMoveAI = FindRandomMove();
                        if(!Posizione[InputMoveAI])//verfica per l'AI le mosse libere
                          break;
                    }
                    AIMenager(InputMoveAI);

                    if(loopen)
                      InputPlayer = InputPlayerMenager();
                  }
                }
              }
            }
            else
            {
              int DoubleMoveE = predictEnemyDoubleMove();
              if(DoubleMoveE >= 0 && GetParallel(DoubleMoveE) != -1 && InputPlayer != 8 && temp != 8)
              {
                InputMoveAI = DoubleMoveE;
                AIMenager(InputMoveAI);

                InputPlayer = InputPlayerMenager(); 

                while(loopen)
                {
                  Possibletris = FindPossibleTris();
                  if(Possibletris >= 0)
                  {
                      InputMoveAI = Possibletris;
                      AIMenager(InputMoveAI); //AI Win
                  }
                  else
                  {
                    BlockEMove = blockEnemy();

                    if(BlockEMove >= 0)
                    {
                      InputMoveAI = BlockEMove;
                      AIMenager(InputMoveAI);

                      if(loopen)
                        InputPlayer = InputPlayerMenager(); 
                    }
                    else
                    {
                      while(true)
                      {
                          InputMoveAI = FindRandomMove();
                          if(!Posizione[InputMoveAI])//verfica per l'AI le mosse libere
                            break;
                      }
                      AIMenager(InputMoveAI);

                      if(loopen)
                        InputPlayer = InputPlayerMenager();
                    }
                  }
                }
              }
              else
              {
                InputMoveAI = GetL(InputPlayer, temp);
                if(InputMoveAI < 0)
                 InputMoveAI = GetL(temp, InputPlayer);

                BlockEMove = blockEnemy();

                if(BlockEMove >= 0)
                {
                  InputMoveAI = GetOpposite(InputMoveAI);
                } 
                AIMenager(InputMoveAI);

                InputPlayer = InputPlayerMenager();

                while(loopen)
                {
                  Possibletris = FindPossibleTris();
                  if(Possibletris >= 0)
                  {
                      InputMoveAI = Possibletris;
                      AIMenager(InputMoveAI); //AI Win
                  }
                  else
                  {
                    BlockEMove = blockEnemy();

                    if(BlockEMove >= 0)
                    {
                      InputMoveAI = BlockEMove;
                      AIMenager(InputMoveAI);

                      if(loopen)
                        InputPlayer = InputPlayerMenager(); 
                    }
                    else
                    {
                      while(true)
                      {
                          InputMoveAI = FindRandomMove();
                          if(!Posizione[InputMoveAI])//verfica per l'AI le mosse libere
                            break;
                      }
                      AIMenager(InputMoveAI);

                      if(loopen)
                        InputPlayer = InputPlayerMenager();
                    }
                  }
                }
              }
            }
          }
        }
    }
    loopen = true;
}

void GetAIMenu()
{
    char input;
    cout<<"\n\n\033[1mInserisci la modalità di AI:\033[0m\n"<<endl<<"1 Random Mode\n2 Medium Mode\n3 Impossibile Mode\n";
    cin>>input;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    switch (input)
    {
        case '1':
            cout<<"\n\n\n\033[107m \033[30mHai scelto la modalità AI Random \033[0m\n\n"; //casuale
            PlayGameRandomAI();
            break;
        case '2':
            cout<<"\n\n\033[107m \033[30mLavori In corso... \033[0m\n\n"; // bravo ma non esperto
            break;
        case '3':
            cout<<"\n\n\033[107m \033[30mHai scelto la modalità AI Impossible \033[0m\n\n"; //modalità impossibile
            PlayGameImpossibleMode();
            break;
        default:
            cout<<"\nOpzione non valida\n\n";
            break;
    }
}

int main()
{
  srand(time(NULL));
  while(GetLoopForGame)//variabile globale true che si occupa di gestire il loop del gioco (si conclude con il tasto 4 nel menu)
    {
      NewGame();//chiamata principale per l'inizio del gioco
    }
}


void GetMenu(char InputModality)
{
  switch(InputModality)//scegliere le opzioni per il menu
    {
      case '1':
        cout<<"\n\nHai scelto la modalità 2 giocatori\n\n"; //1vs1
        PlayGameCoop();
        break;
      case '2':
        cout<<"\n\nHai scelto la modalità contro l'AI\n\n"; //tendina modalità
        GetAIMenu();
        break;
      case '3':
         Credits();//Aiuto sto impazzendo troppo codice, pls help me
        break;
      case '4':
          cout<<"Arrivederci vieni a rigiocare!";
          GetLoopForGame=false;//loop nel main disattivato
        break;
      default:
        cout<<"\nOpzione non valida\n\n";
        break;
    }
}

void NewGame()//variabile che si occupa dell'avvio del gioco
{
  char input;//varibile a disposizione per il menù
  this_thread::sleep_for(chrono::seconds(2));
  cout<<"\033[2J\033[H\n\033[1mBenevenuto su Tic-Tac-Toe, scegli la tua modalità:\033[0m\n"<<endl<<"1 Duel\n2 Challenge with AI\n3 Credits\n4 Quit\n";
  cin>>input;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');//utilizzato per ignorare tutti i caratteri rimanenti nella linea corrente, in modo da preparare il flusso per il prossimo input
  GetMenu(input);//chiamta menù principale
}

void Credits()
{
  string rule[] = {"Sviluppatore", "Game Designer", "Sceneggiatura", "Colonna Sonora Originale", "Design dei Personaggi", "Grafica 3D", "Suoni", "Marketing", "Test di Gioco", "Effetti Sonori", "Animazione", "Effetti Speciali", "Programmatore Principale", "Concept Artist", "Level Designer", "Direzione della Fotografia", "Montaggio", "Regia", "Coordinatore della Produzione", "Direzione Creativa", "Storyboard", "Regia Audio", "Supporto Tecnico", "Programmazione Multiplayer", "Narratore", "Dialoghi", "Effetti Visivi", "Motion Capture", "Composizione Musicale", "Localizzazione", "Coordinatore QA", "Team di Progettazione Ambientale", "Gestione del Progetto", "Regista di Doppiaggio", "Assistente Artistico", "Supervisore QA", "Responsabile delle Pubbliche Relazioni", "Coordinatore Marketing"};
  this_thread::sleep_for(chrono::seconds(1));
  cout<<"\n\n\n";
  for(int i = 0 ;i<37; i++)
  {
    this_thread::sleep_for(chrono::milliseconds(300));
    cout<<"\033[1m"<<rule[i]<<"\033[0m: ";
    this_thread::sleep_for(chrono::milliseconds(200));
    cout<<"\033[91mElia Caviglia\033[0m\n";
    this_thread::sleep_for(chrono::milliseconds(100));
  }
  this_thread::sleep_for(chrono::seconds(1));
  cout<<"\n\n";
}
