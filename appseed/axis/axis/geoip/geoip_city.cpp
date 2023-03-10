#include "framework.h"
#include <stdio.h>
//#include"net/net_sockets.h"
//#include"geoip.h"

static
const int32_t FULL_RECORD_LENGTH = 50;

static_function
GeoIPRecord * _extract_record(GeoIP* gi, uint32_t seek_record, int32_t *next_record_ptr) {
   int32_t record_pointer;
   uchar *record_buf = NULL;
   uchar *begin_record_buf = NULL;
   GeoIPRecord * record;
   int32_t str_length = 0;
   int32_t j;
   double latitude = 0, longitude = 0;
   int32_t metroarea_combo = 0;
   size_t bytes_read = 0;
   if (seek_record == gi->databaseSegments[0])
      return NULL;

   record = (GeoIPRecord *) malloc(sizeof(GeoIPRecord));
   memset(record, 0, sizeof(GeoIPRecord));

   record->charset = gi->charset;

   record_pointer = seek_record + (2 * gi->record_length - 1) * gi->databaseSegments[0];

   if (gi->cache == NULL) {
      fseek(gi->GeoIPDataaxis, record_pointer, SEEK_SET);
      begin_record_buf = record_buf = (uchar *) malloc(sizeof(char) * FULL_RECORD_LENGTH);
      bytes_read = fread(record_buf, sizeof(char), FULL_RECORD_LENGTH, gi->GeoIPDataaxis);
      if (bytes_read == 0) {
         /* eof or other error */
         free(begin_record_buf);
         free(record);
         return NULL;
      }
   } else {
      record_buf = gi->cache + (long)record_pointer;
   }

   /* get country */
   record->continent_code = (char *) GeoIP_country_continent[record_buf[0]];
   record->country_code   = (char *) GeoIP_country_code [record_buf[0]];
   record->country_code3 = (char *) GeoIP_country_code3[record_buf[0]];
   record->country_name   = (char *) GeoIP_country_name [record_buf[0]];
   record_buf++;

   /* get region */
   while (record_buf[str_length] != '\0')
      str_length++;
   if (str_length > 0) {
      record->region = (char *) malloc(str_length+1);
      strncpy(record->region, (char *)record_buf, str_length+1);
   }
   record_buf += str_length + 1;
   str_length = 0;

   /* get city */
   while (record_buf[str_length] != '\0')
      str_length++;
   if (str_length > 0) {
      if ( gi->charset == GEOIP_CHARSET_UTF8 ) {
         record->city = _iso_8859_1__utf8( (const char * ) record_buf );
      } else {
         record->city = (char *) malloc(str_length+1);
         strncpy(record->city, ( const char * ) record_buf, str_length+1);
      }
   }
   record_buf += (str_length + 1);
   str_length = 0;

   /* get postal code */
   while (record_buf[str_length] != '\0')
      str_length++;
   if (str_length > 0) {
      record->postal_code = (char *) malloc(str_length+1);
      strncpy(record->postal_code, (char *)record_buf, str_length+1);
   }
   record_buf += (str_length + 1);

   /* get latitude */
for (j = 0; j < 3; ++j)
      latitude += (record_buf[j] << (j * 8));
   record->latitude = (float) (latitude/10000 - 180);
   record_buf += 3;

   /* get longitude */
   for (j = 0; j < 3; ++j)
      longitude += (record_buf[j] << (j * 8));
   record->longitude = (float) (longitude/10000 - 180);

   /* get area code and metro code for post April 2002 databases and for US locations */
   if ((char) GEOIP_CITY_EDITION_REV1 == gi->databaseType) {
      if (!strcmp(record->country_code, "US")) {
         record_buf += 3;
         for (j = 0; j < 3; ++j)
            metroarea_combo += (record_buf[j] << (j * 8));
         record->metro_code = metroarea_combo/1000;
         record->area_code = metroarea_combo % 1000;
      }
   }

   if (gi->cache == NULL)
      free(begin_record_buf);

   /* Used for GeoIP_next_record */
   if (next_record_ptr != NULL)
      *next_record_ptr = (int32_t) (seek_record + record_buf - begin_record_buf + 3);

   return record;
}

static_function
GeoIPRecord * _get_record(GeoIP* gi, uint32_t ipnum) {
   uint32_t seek_record;

   if (gi->databaseType != (char) GEOIP_CITY_EDITION_REV0 &&
         gi->databaseType != (char) GEOIP_CITY_EDITION_REV1) {
      debug_print("Invalid database type %s, expected %s\n", GeoIPDBDescription[(int32_t)gi->databaseType], GeoIPDBDescription[GEOIP_CITY_EDITION_REV1]);
      return 0;
   }

   seek_record = _GeoIP_seek_record(gi, ipnum);
   return _extract_record(gi, seek_record, NULL);
}

static_function
GeoIPRecord * _get_record_v6(GeoIP* gi, geoipv6_t ipnum) {
       uint32_t seek_record;

       if (gi->databaseType != (char) GEOIP_CITY_EDITION_REV0 &&
                       gi->databaseType != (char) GEOIP_CITY_EDITION_REV1) {
               debug_print("Invalid database type %s, expected %s\n", GeoIPDBDescription[(int32_t)gi->databaseType], GeoIPDBDescription[GEOIP_CITY_EDITION_REV1]);
               return 0;
       }

       seek_record = _GeoIP_seek_record_v6(gi, ipnum);
       return _extract_record(gi, seek_record, NULL);
}



GeoIPRecord * GeoIP_record_by_ipnum (GeoIP* gi, uint32_t ipnum) {
   return _get_record(gi, ipnum);
}

GeoIPRecord * GeoIP_record_by_ipnum_v6 (GeoIP* gi, geoipv6_t ipnum) {
       return _get_record_v6(gi, ipnum);
}

GeoIPRecord * GeoIP_record_by_addr (GeoIP* gi, const char *addr) {
   uint32_t ipnum;
   if (addr == NULL) {
      return 0;
   }
   ipnum = _GeoIP_addr_to_num(addr);
   return _get_record(gi, ipnum);
}

GeoIPRecord * GeoIP_record_by_addr_v6 (GeoIP* gi, const char *addr) {
       geoipv6_t ipnum;
       if (addr == NULL) {
               return 0;
       }
       ipnum = _GeoIP_addr_to_num_v6(addr);
       return _get_record_v6(gi, ipnum);
}

GeoIPRecord * GeoIP_record_by_name (GeoIP* gi, const char *name) {
   uint32_t ipnum;
   if (name == NULL) {
      return 0;
   }
   ipnum = _GeoIP_lookupaddress(name);
   return _get_record(gi, ipnum);
}

GeoIPRecord * GeoIP_record_by_name_v6 (GeoIP* gi, const char *name) {
        geoipv6_t ipnum;
       if (name == NULL) {
               return 0;
       }
       ipnum = _GeoIP_lookupaddress_v6(name);
       return _get_record_v6(gi, ipnum);
}

int32_t GeoIP_record_id_by_addr (GeoIP* gi, const char *addr) {
   uint32_t ipnum;
   if (gi->databaseType != (char) GEOIP_CITY_EDITION_REV0 &&
         gi->databaseType != (char) GEOIP_CITY_EDITION_REV1) {
      debug_print("Invalid database type %s, expected %s\n", GeoIPDBDescription[(int32_t)gi->databaseType], GeoIPDBDescription[GEOIP_CITY_EDITION_REV1]);
      return 0;
   }
   if (addr == NULL) {
      return 0;
   }
   ipnum = _GeoIP_addr_to_num(addr);
   return _GeoIP_seek_record(gi, ipnum);
}

int32_t GeoIP_record_id_by_addr_v6 (GeoIP* gi, const char *addr) {
       geoipv6_t ipnum;
       if (gi->databaseType != (char) GEOIP_CITY_EDITION_REV0 &&
                       gi->databaseType != (char) GEOIP_CITY_EDITION_REV1) {
               debug_print("Invalid database type %s, expected %s\n", GeoIPDBDescription[(int32_t)gi->databaseType], GeoIPDBDescription[GEOIP_CITY_EDITION_REV1]);
               return 0;
       }
       if (addr == NULL) {
               return 0;
       }
       ipnum = _GeoIP_addr_to_num_v6(addr);
       return _GeoIP_seek_record_v6(gi, ipnum);
}

int32_t GeoIP_init_record_iter (GeoIP* gi) {
   return gi->databaseSegments[0] + 1;
}

int32_t GeoIP_next_record (GeoIP* gi, GeoIPRecord **gir, int32_t *record_iter) {
   if (gi->cache != NULL) {
      debug_print("GeoIP_next_record not supported in memory cache mode\n");
      return 1;
   }
   *gir = _extract_record(gi, *record_iter, record_iter);
   return 0;
}

void GeoIPRecord_delete (GeoIPRecord *gir) {
   free(gir->region);
   free(gir->city);
   free(gir->postal_code);
   free(gir);
}



char * _iso_8859_1__utf8(const char * iso)
{

   signed char ca;
   char k;
   char * p;
   char * t = (char *)iso;
   strsize len = 0;
   while ( ( ca = *t++) )
   {
      if ( ca < 0 )
         len++;
   }
   len += t - iso;
   t = p = (char *) malloc( len );

   if ( p ){
      while ( ( ca = *iso++ ) ) {
         if (ca < 0 ) {
            k = '\xc2';
            if (ca >= -64 )
               k++;
            *t++ = k;
            ca &= ~0x40;
         }
         *t++ = ca;
      }
      *t++ = 0x00;
   }
   return p;
}
