#include<stdio.h>
#include <string.h>
#include <fstream>  // Función de biblioteca de flujo de archivos
#include <iostream>

using namespace std;
string tipo = "";
char palabrasReservadas[33][20]={ // Codificación de palabra reservada 1-32
    "auto", "break", "case", "char", "const", "continue",
    "default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "int", "long",
    "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while"
};

char operadorSimple[24][10]={  // Codificación de operador de un solo byte 33-57
    "+", "-", "*", "/", "<", ">",
    ";", "(", ")", "^", ",", "#",
    "|", "%", "~", "[", "]", "{",
    "}", ".", ":", "!", "=", "&"
};

static char operadorDoble[14][10]={  // Codificación de operador de doble byte 57-71
    "++","--","==","<=",">=","<<",
    ">>","&&","||","!=","\\","\?",
    "\"", "\'"
};


	// Determinar si es una letra
	int isLetter(char ch){
	    if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')){return 1;}
	    return 0;
	}
	
	// Determina si es un número
	int isDigit(char ch){
	    if((ch>='0'&&ch<='9')){return 1;}
	    return 0;
	}
	// Determinar si es un operador y devolver el valor relativo del código
	int isOperater(char ch){
	    int i;
	    for(i=0;i<sizeof(operadorSimple)/10;i++){
	        if(ch==operadorSimple[i][0]){
	            return i;
	        }
	    }
	    return -1;
	}
	// Determinar si es una palabra reservada y devolver el valor relativo del código
	int isReserve(char token[]){
	    int i;
	    
	    for(i=0;i<sizeof(palabrasReservadas)/20;i++){
	        if(strcmp(token,palabrasReservadas[i])==0){
	            return i;
	        }
	    }
	    return 100;
	}
// Puntero hacia atrás
void Retract(int &pProject){
    pProject--;
    //cout<<pProject;
}

// Escanea una palabra a la vez
void Scanner(int& syn,int &pProject,char token[],char projectResorce[]){
	
    char ch,mutil[2]={"\0"};
    int ptoken=0,i=0;
    syn=0;
    ch=projectResorce[pProject++];
    
    while(ch==' '){ // Limpia el espacio antes de la palabra
        ch=projectResorce[pProject++];
    }
    for (i = 0; i<20; i++){// Limpia el token antes de cada colección
        token[i] = '\0';
    }
    
    if(isLetter(ch)){  // Identificador o reconocimiento de palabra reservada
        while(isLetter(ch)||isDigit(ch)){
            token[ptoken++]=ch;
            ch=projectResorce[pProject++];
        }
        
        Retract(pProject);
        syn=isReserve(token);
//        cout<<"token syn " <<token<<syn<<endl;
        if(syn==100){ // La palabra es un identificador
            return ;
        }
        syn+=1;
        tipo = "Palabra reservada";
        return ;  // Devuelve el código de categoría
    }
    else if(isDigit(ch)){ // Reconocimiento constante
        while(isDigit(ch)){
            token[ptoken++]=ch;
            ch=projectResorce[pProject++];
        }
        syn=99; // Devuelve el código de categoría
        Retract(pProject);
        return ;
    }
    else if((syn=isOperater(ch))!=-1){// Después del juicio y obtener el valor relativo del código de tipo de carácter único
        token[ptoken++]=ch;
        mutil[0]=ch;    // Almacena el contenido después del operador
        mutil[1]=projectResorce[pProject++];
        for(i=0;i<sizeof(operadorDoble)/10;i++){// Determine si es un operador de doble byte
            if(strcmp(mutil,operadorDoble[i])==0){
                syn=58+i;
                token[ptoken++]=ch;
                tipo = "Operador doble";
                return ;
            }
        }
        Retract(pProject);
        syn=33+syn;
        tipo = "Operador simple";
        return ;
    }else if(ch=='\"'){
    	do{
            token[ptoken++]=ch;
            ch=projectResorce[pProject++];
        }while(ch!='\"');
        token[ptoken++]='\"';
        //Retract(pProject);
        syn = 90;
        return ;
	}
    else if(ch=='$'){syn=0;}
    else{syn=-1;}
    return ;
}

int main(){
	
	ofstream file;
  	file.open("archivo.txt");
  	
	file<<"|-------------------------------------------------------------------------------------------------------|"<<endl;
	file<<"|                              ANALIZADOR LEXICO - JULIAN M - DANIEL C                                  |"<<endl;
	file<<"|-------------------------------------------------------------------------------------------------------|"<<endl;
	file<<"|    LEXEMA   \t\t\t\t|    		CATEGORIA     			|     ID        |"<<endl;
	file<<"________________________________________________________________________________________________________"<<endl;
	
	
    int pProject=0,syn=-1;
    char token[20],matrizComp[1000];
    FILE *fp;
    
    fp=fopen("test.txt","r");
    do{
    	fgets(matrizComp,1000,fp);// Obtener el contenido del archivo a la matriz
	
   
    //while (fgets(matrizComp,1000,fp))
	//strtok(matrizComp, "\n");
      
    
    cout<<"linea escaneada:\n"<<matrizComp<<endl;
    pProject=0; // Juzgar desde cero
    while(syn!=0){ // Salida binaria
    	tipo = "";
        Scanner(syn,pProject,token,matrizComp);
        if(syn==99){
        	file<<"|   "<<token<<"    \t\t\t\t|       Constante   \t\t\t\t|    "<<syn<<"   \t|"<<endl;
        	file<<"--------------------------------------------------------------------------------------------------------"<<endl;
            cout<<"<Constante,"<<token<<">"<<endl;
        }else if(syn==100){
        	file<<"|   "<<token<<"    \t\t\t\t|       Identificador   \t\t\t|    "<<syn<<"   \t|"<<endl;
        	file<<"--------------------------------------------------------------------------------------------------------"<<endl;
            cout<<"<Identificador "<<token<<">"<<endl;
        }else if(syn>=1&&syn<=72){
        	file<<"|   "<<token<<"    \t\t\t\t|       "<<tipo<<"  \t\t\t|    "<<syn<<"   \t|"<<endl;
        	file<<"--------------------------------------------------------------------------------------------------------"<<endl;
            cout<<"<"<<token<<",-->  ("<<syn<<")"<<endl;
        }else if(syn==90){
        	file<<"|   "<<token<<"    \t\t\t|       Literal  \t\t\t|    "<<syn<<"   \t|"<<endl;
        	file<<"--------------------------------------------------------------------------------------------------------"<<endl;
            cout<<"<Literal "<<token<<",-->  ("<<syn<<")"<<endl;
        }else if(syn==-1){
            //cout<<"error!\n"<<endl;
            break;
        }
        else{ cout<<"Analisis terminado\n"<<endl; }
        // Salida y escritura de archivos
    }
    }while(!feof(fp));
    fclose(fp);
    return 0;
}



