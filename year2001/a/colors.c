#include <stdio.h>

long distsq( long x1, long y1, long z1, long x2, long y2, long z2 ) {
  return (x2-x1) * (x2-x1) + (y2-y1) * (y2-y1) + (z2-z1) * (z2-z1);
}

int main( void ) {
   struct { int r, g, b; } c[ 16 ];
   int i, j, r, g, b;
   long k;
   FILE *in = fopen( "colors.in", "r" ), *out = fopen( "colors.out", "w" );
   for (i = 0; i < 16; i++)
      fscanf( in, "%d %d %d\n", &c[ i ].r, &c[ i ].g, &c[ i ].b );
   while ( fscanf( in, "%d %d %d\n", &r, &g, &b ) != EOF && r != -1 ) {
      k = 999999L;
      for (i = 0; i < 16; i++)
         if (distsq( r, g, b, c[ i ].r, c[ i ].g, c[ i ].b ) < k) {
            j = i;
            k = distsq( r, g, b, c[ i ].r, c[ i ].g, c[ i ].b );
         }
      fprintf( out, "(%d,%d,%d) maps to (%d,%d,%d)\n", r, g, b,
               c[j].r, c[j].g, c[j].b );
   }
   fclose( in );
   fclose( out );
   return 0;
}
