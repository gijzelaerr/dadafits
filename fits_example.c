#include <string.h>
#include <stdio.h>
#include "fitsio.h"

void print_table(fitsfile *fptr) {
  int status, tstatus;

  long nrows;
  fits_get_num_rows(fptr, &nrows, &status);

  int ncols, c;
  fits_get_num_cols(fptr, &ncols, &status);

  printf("Table contains %li rows and %i columns\n", nrows, ncols);

  char colname[256];
  int typecode;
  long repeat, width;

  fits_get_colname(fptr, CASEINSEN, "*", colname, &c, &status);
  while(status != COL_NOT_FOUND) {
    fits_get_coltype(fptr, c, &typecode, &repeat, &width, &tstatus);
    switch (typecode) {
      case TSTRING:     printf("%03i %10s TSTRING %li %li\n", c, colname, repeat, width); break;
      case TSHORT:      printf("%03i %10s TSHORT %li %li\n", c, colname, repeat, width); break;
      case TLONG:       printf("%03i %10s TLONG %li %li\n", c, colname, repeat, width); break;
      case TFLOAT:      printf("%03i %10s TFLOAT %li %li\n", c, colname, repeat, width); break;
      case TDOUBLE:     printf("%03i %10s TDOUBLE %li %li\n", c, colname, repeat, width); break;
      case TLOGICAL:    printf("%03i %10s TLOGICAL %li %li\n", c, colname, repeat, width); break;
      case TBIT:        printf("%03i %10s TBIT %li %li\n", c, colname, repeat, width); break;
      case TBYTE:       printf("%03i %10s TBYTE %li %li\n", c, colname, repeat, width); break;
      case TCOMPLEX:    printf("%03i %10s TCOMPLEX %li %li\n", c, colname, repeat, width); break;
      case TDBLCOMPLEX: printf("%03i %10s TDBLCOMPLEX %li %li\n", c, colname, repeat, width); break;
      // case TINT32BIT:   printf("%03i TINT32BIT %li %li\n", c, colname, repeat, width); break;
      default:          printf("%03i %10s Unknown typecode\n", c, colname); break;
    }
    fits_get_colname(fptr, CASEINSEN, "*", colname, &c, &status);
  }
}

void print_hdus(fitsfile *fptr) {
  int hdunum;
  int status;

  fits_get_num_hdus(fptr, &hdunum, &status);
  printf("File contains %i HDUs\n", hdunum);

  int h;
  for (h=1; h <= hdunum; h++) {
    int type;
    fits_movabs_hdu(fptr, h, &type, &status);
    switch(type) {
      case IMAGE_HDU:
        printf("%i is an IMAGE_HDU\n", h);
        break;
      case ASCII_TBL:
        printf("%i is an ASCII_TBL\n", h);
        print_table(fptr);
        break;
      case BINARY_TBL:
        printf("%i is an BINARY_TBL\n", h);
        print_table(fptr);
        break;
      default:
        printf("%i is an unknown type\n", h);
        break;
    }
  }
  printf("Status is %i\n", status);
}

int main(int argc, char *argv[]) {
  int status;
  fitsfile *fptr;

  fits_create_file(&fptr, "example.fits.gz(template.txt)", &status);
 
  fits_movabs_hdu(fptr, 1, NULL, &status);
  fits_write_date(fptr, &status);
  fits_write_chksum(fptr, &status);
 
  fits_movabs_hdu(fptr, 2, NULL, &status);
  fits_insert_rows(fptr, 0, 1, &status);
 
  char bits[48000];
  fits_write_col(fptr,
      TBYTE,        // type
      17,           // colnum
      1,            // firstrow
      1,            // firstelem
      48000,        // nelement
      &bits,        // *array
      &status       // *status
  );
 
  fits_write_chksum(fptr, &status);
  fits_close_file (fptr, &status);
}

/*
  fits_write_col(fptr, TDOUBLE, 2, rowid, 1, 1, offs_sub, &status);
  fits_write_col(fptr, TDOUBLE, 3, rowid, 1, 1, lst_sub,  &status);
  fits_write_col(fptr, TDOUBLE, 4, rowid, 1, 1, ra_sub,   &status);
  fits_write_col(fptr, TDOUBLE, 5, rowid, 1, 1, dec_sub,  &status);
  fits_write_col(fptr, TDOUBLE, 6, rowid, 1, 1, glon_sub, &status);
  fits_write_col(fptr, TDOUBLE, 7, rowid, 1, 1, glat_sub, &status);

  fits_write_col(fptr, TFLOAT,  8, rowid, 1, 1, fd_ang,  &status);
  fits_write_col(fptr, TFLOAT,  9, rowid, 1, 1, pos_ang, &status);
  fits_write_col(fptr, TFLOAT, 10, rowid, 1, 1, par_ang, &status);
  fits_write_col(fptr, TFLOAT, 11, rowid, 1, 1, tel_az,  &status);
  fits_write_col(fptr, TFLOAT, 12, rowid, 1, 1, tel_zen, &status);

  fits_write_col(fptr, TFLOAT, 13, rowid, 1, 384, dat_freq, &status);
  fits_write_col(fptr, TFLOAT, 14, rowid, 1, 384, dat_wts,  &status);
  fits_write_col(fptr, TFLOAT, 15, rowid, 1, 384, dat_offs, &status);
  fits_write_col(fptr, TFLOAT, 16, rowid, 1, 384, dat_scl,  &status);

  fits_write_col(fptr, TBYTE, 17, rowid, 1, 48000, data, &status);
*/

