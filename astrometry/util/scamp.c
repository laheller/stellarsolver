/*
 # This file is part of the Astrometry.net suite.
 # Licensed under a 3-clause BSD style license - see LICENSE
 */

#include "scamp.h"
#include "scamp-catalog.h"
#include "sip_qfits.h"
#include "errors.h"
#include "log.h"

int scamp_write_field(const qfits_header* imageheader,
                      const sip_t* wcs,
                      const starxy_t* xy,
                      const char* filename) {
    scamp_cat_t* scamp;
    qfits_header* hdr;
    int i;

    if (!imageheader)
        hdr = qfits_table_prim_header_default();
    else
        hdr = qfits_header_copy(imageheader);

    sip_add_to_header(hdr, wcs);

    scamp = scamp_catalog_open_for_writing(filename, FALSE);
    if (!scamp) {
        return -1;
    }

    if (scamp_catalog_write_field_header(scamp, hdr)) {
        return -1;
    }
    qfits_header_destroy(hdr);

    for (i=0; i<starxy_n(xy); i++) {
        scamp_obj_t obj;
        obj.x = starxy_getx(xy, i);
        obj.y = starxy_gety(xy, i);
        obj.err_a = 1.0;
        obj.err_b = 1.0;
        obj.err_theta = 0.0;
        if (xy->flux)
            obj.flux = xy->flux[i];
        else
            obj.flux = 1000.0;
        obj.err_flux = 1.0;
        obj.flags = 0;
        if (scamp_catalog_write_object(scamp, &obj)) {
            return -1;
        }
    }

    if (scamp_catalog_close(scamp)) {
        return -1;
    }
    return 0;
}

int scamp_write_config_file(const char* refcatfn, const char* outfn) {
    FILE* fid;
    char* text;
    int res;
    fid = fopen(outfn, "w");
    if (!fid) {
        SYSERROR("Failed to open file %s to write Scamp config file", outfn);
        return -1;
    }
    text = scamp_get_config_options(refcatfn);
    res = fprintf(fid, "%s", text);
    free(text);
    if (res < 0) {
        SYSERROR("Failed to write to Scamp config file %s", outfn);
        return -1;
    }
    if (fclose(fid)) {
        SYSERROR("Failed to close to Scamp config file %s", outfn);
        return -1;
    }
    return 0;
}

char* scamp_get_config_options(const char* refcatfn) {
    char* res;
    char* fmt =
        "## These are SCAMP config file entries to get Scamp to read the\n"
        "## catalogs generated by the Astrometry.net Scamp integration code.\n"
        "#\n"
        "# The following decribe the reference catalog:\n"
        "# Read from a local file.\n"
        "ASTREF_CATALOG    FILE\n"
        "# You can also set this to: USNO-B1, SDSS-R7, 2MASS, etc\n"
        "# The reference catalog file name is:\n"
        "ASTREFCAT_NAME    %s\n"
        "# The reference catalog (RA,Dec) in degrees: column names in file\n"
        "ASTREFCENT_KEYS   RA, DEC\n"
        "# The reference catalog (RA,Dec) error ellipse: column names\n"
        "ASTREFERR_KEYS    ERR_A, ERR_B\n"
        "# The reference catalog magnitude: column names\n"
        "ASTREFMAG_KEY     MAG\n"
        "#\n"
        "# The following decribe the input catalog:\n"
        "#\n"
        "# The (x,y) source positions in pixels: column names\n"
        "CENTROID_KEYS     X_IMAGE, Y_IMAGE\n"
        "# The source positions should get distortions\n"
        "DISTORT_KEYS      X_IMAGE, Y_IMAGE\n"
        "# The error ellipse in source positions: column names\n"
        "CENTROIDERR_KEYS  ERR_A, ERR_B\n"
        "# The flux: column name\n"
        "PHOTFLUX_KEY      FLUX\n"
        "# The flux error: column name\n"
        "PHOTFLUXERR_KEY   FLUX_ERR\n"
        "# Don't run Scamp's matching procedure to get initial WCS\n"
        "# (use the Astrometry.net solution!)\n"
        "MATCH N\n"
        "#\n"
        "## End of Astrometry.net Scamp integration config items\n"
        ;
    asprintf_safe(&res, fmt, refcatfn);
    return res;
}

