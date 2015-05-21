#include <stdio.h>
#include <string.h>

#define FALSE  0
#define TRUE   1
#define STRSIZ 128

typedef char string[ STRSIZ ];

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

void add( const char *a, const char *b, char *c ) {

   int i = strlen( a ), j = strlen( b ), k = 0, l = 0;
   while (i + j > 0) {
      c[ k ] = a[ --i ] + b[ --j ] + l - '0';
      l = 0;
      while (c[ k ] > '9') { c[ k ] -= 10; l += 1; }
      c[ ++k ] = '\0';
   }
   for (i = 0, j = strlen( c ) - 1; i < strlen( c ) / 2; i++, j--) {
      k = c[ i ];
      c[ i ] = c[ j ];
      c[ j ] = k;
   }
}

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

int cycle( const char *a, const char *b ) {

   string c;
   strcpy( c, a );
   strcat( c, a );
   return strstr( c, b ) != NULL;
}

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

int cyclic( const char *a ) {

   string b, c;
   int i;
   strcpy( b, a );
   for (i = 1; i < strlen( a ); i++ ) {
      add( a, b, c );
      if (!cycle( a, c )) return FALSE;
      strcpy( b, c );
   }
   return TRUE;
}

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

int main( void ) {

   string s;
   FILE *in = fopen( "cyclic.in", "r" ), *out = fopen( "cyclic.out", "w" );
   while (fgets( s, STRSIZ, in ) != NULL) {
      s[ strlen( s ) - 1 ] = '\0';
      fprintf( out, "%s is%scyclic\n", s, cyclic( s ) ? " " : " not " );
   }
   fclose( in );
   fclose( out );
   return 0;
}
