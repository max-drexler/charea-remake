"""
An almost 1-to-1 python remake of charea.c

area file parsing is based on McIDAS documentation:
https://www.ssec.wisc.edu/mcidas/doc/prog_man/current/formats-1.html#13797
"""

__author__ = 'Max Drexler'
__email__ = 'mndrexler@wisc.edu'

import argparse
import array
from datetime import datetime
import sys
from typing import List, Tuple

# Mapping between satellite number and satellite name
SENSORS = [
      "Derived_Data", "Test_Pattern", "graphics",      "Misc",       "PDUS_MET_VIS", "PDUS_MET_IR", # /*0-5    */
      "PDUS_MET_WV",  "Radar",        "Misc_Aircraft", "Raw_MET",    "composite",    "blank",   # /*6-11   */
      "GMS_VIS",      "GMS_IR",       "ATS6-VIS",      "ATS6-IR",    "sms1-vis",     "sms1-ir", # /*12-17  */
      "sms2-vis",     "sms2-ir",      "goes1-vis",     "goes1-ir",   "goes2-vis",    "goes2-ir",    # /*18-23  */
      "goes3-vis",    "goes3-ir",     "goes4-vis",     "goes4-ir",   "goes5-vis",    "goes5-ir",    # /*24-29  */
      "goes6-vis",    "goes6-ir",     "goes7-vis",     "goes7-ir",   "fy-2b",        "fy-2c",   # /*30-35  */
      "fy-2d",        "fy-2e",        "fy-2f",         "fy-2g",      "fy-2h",        "tirosn",  # /*36-41  */
      "noaa6",        "noaa7",        "noaa8",         "noaa9",      "venus",        "voyager1",    # /*42-47  */
      "voyager2",     "galileo",      "hubble",        "met8",       "met9",         "met10",   # /*48-53  */
      "met3",         "met4",         "met5",          "met6",       "met7",         "blank",   # /*54-59  */
      "noaa10",       "noaa11",       "noaa12",        "noaa13",     "noaa14",       "noaa15",  # /*60-65  */
      "noaa16",       "noaa17",       "noaa18",        "noaa19",     "goes8",        "goes8sndr",   # /*66-71  */
      "goes9",        "goes9sndr",    "goes10",        "goes10sndr", "goes11",       "goes11sndr",  # /*72-77  */
      "goes12",       "goes12sndr",   "erbe",          "blank",      "gms4",         "gms5",    # /*78-83  */
      "mtsat-1r",     "mtsat-2",      "himawari-8",       "dmspf8",     "dmspf9",       "dmspf10",  # /*84-89  */
      "dmspf11",      "dmspf12",      "dmspf13",       "dmspf14",    "dmspf15",      "fy1b",    # /*90-95  */
      "fy1c",         "fy1d",         "blank",         "blank",      "blank",        "terra",   # /*96-101 */
      "terra-cld",    "terra-geo",    "terra-aer",     "blank",      "terra-top",    "terra-atm",   # /*102-107*/
      "terra-gus",    "terra-ret",    "blank",         "aqua",       "aqua-cld",     "aqua-geo",    # /*108-113*/
      "aqua-aer",     "blank",        "aqua-top",      "aqua-atm",   "aqua-gus",     "aqua-ret",    # /*114-119*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*120-125*/
      "blank",        "blank",        "terra-sst",     "terra-lst",  "blank",        "blank",   # /*126-131*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*132-137*/
      "aqua-sst",     "aqua-lst",     "blank",         "blank",      "blank",        "blank",   # /*138-143*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*144-149*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*150-155*/
      "blank",        "blank",        "blank",         "blank",      "terra-ndvi",   "terra-cref",  # /*156-161*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*162-167*/
      "blank",        "blank",        "aqua-ndvi",     "aqua-cref",  "blank",        "blank",   # /*168-173*/
      "ewsg1",        "ewsg1sndr",    "ewsg2",         "ewsg2sndr",  "ewsg3",        "ewsg3sndr",   # /*174-179*/
      "goes13",       "goes13sndr",   "goes14",        "goes14sndr", "goes15",       "goes15sndr",  # /*180-185*/
      "goes16",       "goes16-l2",    "goes17",        "goes17-l2",  "goes18",       "goes18-l2",   # /*186-191*/
      "goes19",       "goes19-l2",    "blank",         "dmsp_f16",   "dmsp_f17",     "blank",   # /*192-197*/
      "blank",        "blank",        "airs-l1b",      "blank",      "blank",        "blank",   # /*198-203*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*204-209*/
      "amsre-l1b",    "amsre-rain",   "blank",         "blank",      "blank",        "blank",   # /*210-215*/
      "amsu_a-lwp",   "blank",        "blank",         "blank",      "trmm",         "gms1",    # /*216-221*/
      "gms2",         "gms3",         "blank",         "blank",      "blank",        "blank",   # /*222-227*/
      "blank",        "blank",        "kalpana1",      "insat3d-img","insat3d-snd",  "blank",   # /*228-233*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*234-239*/
      "metopa",       "metopb",       "metopc",        "blank",      "blank",        "blank",   # /*240-245*/
      "blank",        "blank",        "blank",         "blank",      "coms1",        "blank",   # /*246-251*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*252-257*/
      "blank",        "blank",        "blank",         "landsat1",   "landsat2",     "landsat3",    # /*258-263*/
      "landsat4",     "landsat5",     "landsat6",      "landsat7",   "landsat8",     "blank",   # /*264-269*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "fy3a",    # /*270-275*/
      "fy3b",         "fy3c",         "blank",         "blank",      "blank",        "blank",   # /*276-281*/
      "blank",        "blank",        "blank",         "blank",      "himawaricast8","himawari-9",  # /*282-287*/
      "himawaricast9","himawaricast8/9","blank",       "blank",      "blank",        "blank",   # /*288-293*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*294-299*/
      "npp-viirs",    "noaa20",       "noaa21",        "noaa22",     "noaa23",       "blank",   # /*300-305*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*306-311*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*312-317*/
      "blank",        "blank",        "snppsdr",       "noaa20sdr",  "noaa21sdr",    "noaa22sdr",   # /*318-223*/
      "noaa23sdr",    "snppedr",      "noaa20edr",     "noaa21edr",  "noaa22edr",    "noaa23edr",   # /*324-329*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*330-335*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*336-341*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*342-347*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*348-353*/
      "met11",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*354-359*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*360-365*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*366-371*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*372-377*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*378-383*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*384-389*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*390-395*/
      "blank",        "blank",        "blank",         "blank",      "s_pole_comp",  "n_pole_comp", # /*396-401*/
      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",   # /*402-407*/
      "blank",        "blank",        "megha-tropic",  "blank",      "blank",        "blank",   # /*408-413*/
]


def parse_date(yyyddd: int) -> Tuple[str, str]:
    """Gets the year and the day of year based on an area file's entry

    area file's dates are yyyddd where yyy == yyyy % 1900
    """
    yyy, doy = divmod(yyyddd, 1000)
    return str(1900 + yyy), str(doy).rjust(3, '0')


def get_bands(num_bands: int, band_map1: int, band_map2: int) -> str:
    """Gets the bands from an area file.

    Getting bands from an area file is a multi-step process.
    band_map1 and 2 are integer conversions of 32bit fields of the area file,
    those 32bit fields are the key to getting all bands from the area file.
    The index+1 of any 1 in the 32 bits represents a present band.
    e.x. 32bit field = '10100000000000000000000000000000' -> band 1 and 3

    If there are more than 32 bands present, band_map2 is also used.
    """
    def bands_from_int(b_int: int, offset: int):
        # turns integer into a binary string
        bit_field = f'{b_int:032b}'[::-1]
        for i, ch in enumerate(bit_field):
            if ch == '1':
                yield str(i+offset)
    bands = [band for band in bands_from_int(band_map1, 1)]
    if num_bands > 32:
        bands.extend([band for band in bands_from_int(band_map2, 33)])
    return ''.join(bands)


def main(argc: int, argv: List[str]) -> int:

    parser = argparse.ArgumentParser()
    parser.add_argument('file', help='an area file')
    parser.add_argument('-m', action='store_true', help='print month name')
    parser.add_argument('-g', action='store_true', help='use gregorian date fmt')
    args = parser.parse_args()

    area_file_path = args.file
    print_month = args.m
    print_greg = args.g

    # Store area file directory block as an array of integers
    dir_block = array.array('i')

    area_file = None
    try:
        area_file = open(area_file_path, 'rb')
        dir_block.fromfile(area_file, 64)
    except OSError:
        print("\nusage:charea [-mg] AREA0001\n")
        return 1
    finally:
        if area_file is not None:
            area_file.close()

    # see if bits are flipped
    if dir_block[1] != 4:
        dir_block.byteswap()

    # load data from directory block
    year, doy = parse_date(dir_block[3])
    time = str(dir_block[4]).rjust(6, '0')
    date = datetime.strptime(''.join((year, doy, time)), '%Y%j%H%M%S')
    satellite = SENSORS[dir_block[2]]

    # construct file's name
    name = f'{satellite}_'

    if print_month:
        name += f"{date.strftime('%b').lower()}_"

    if 'sndr' not in satellite:
        # add band number(s)
        name += f"{get_bands(dir_block[13], dir_block[18], dir_block[19])}_"

    # add year
    name += f"{date.strftime('%Y')}_"

    # add julian or gregorian date
    if print_greg:
        name += f"{date.strftime('%m_%d')}_"
    else:
        name += f"{date.strftime('%j')}_"

    # add time
    name += f"{date.strftime('%H%M')}"

    print(name)


if __name__ == '__main__':
    sys.exit(main(len(sys.argv), sys.argv))
