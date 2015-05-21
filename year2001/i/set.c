#include <stdio.h>

#define FALSE 0
#define TRUE  1

char card[ 12 ][ 4 ];
int set[ 3 ];
FILE *in, *out;

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

int readcards( void ) {

   int i;

   for (i = 0; i < 12; i++)
      if (fscanf( in, "%c%c%c%c\n", &card[ i ][ 0 ], &card[ i ][ 1 ],
                  &card[ i ][ 2 ], &card[ i ][ 3 ] ) == EOF) return FALSE;
   fscanf( in, "\n" );
   return TRUE;
}

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

int isaset( void ) {

   int i, a = set[ 0 ], b = set[ 1 ], c = set[ 2 ];

   for (i = 0; i < 4; i++) {
      if (card[ a ][ i ] == card[ b ][ i ]) {
         if (card[ b ][ i ] != card[ c ][ i ]) return FALSE;
      }
      else if (card[ a ][ i ] == card[ c ][ i ] ||
               card[ b ][ i ] == card[ c ][ i ])
         return FALSE;
   }
   return TRUE;
}

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

int nextset( void ) {

   int i, j;

   for (i = 2; i > -1; i--)
      if (set[ i ] < 9 + i) {
         set[ i ]++;
         for (j = i + 1; j < 3; j++) set[ j ] = set[ j - 1 ] + 1;
         return TRUE;
      };
   return FALSE;
}

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

int main( void ) {

   int i, j, n;

   in = fopen( "set.in", "r" );
   out = fopen( "set.out", "w" );
   while (readcards()) {
      fprintf( out, "CARDS: " );
      for (i = 0; i < 12; i++)
         for (j = 0; j < 4; j++)
            fprintf( out, j ? "%c" : " %c", card[ i ][ j ] );
      fprintf( out, "\n" );
      for (i = 0; i < 3; i++) set[ i ] = i;
      n = 0;
      do {
         if (isaset()) {
            fprintf( out, n ? "      %3d. " : "SETS: %3d. ", n + 1 );
            for (i = 0; i < 3; i++)
               for (j = 0; j < 4; j++)
                  fprintf( out, j ? "%c" : " %c", card[ set[ i ] ][ j ] );
            fprintf( out, "\n" );
            n++;
         }
      } while (nextset());
      fprintf( out, n ? "\n" : "SETS:   *** None Found ***\n\n" );
   }
   fclose( in );
   fclose( out );
   return 0;
}
