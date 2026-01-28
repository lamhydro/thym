# TROPICAL HYDROLOGICAL MODELLING (THYM) PLATFORM
---

## Description
---

**thym** is hydrological modelling platform  designed to analyse hydrological processes in tropical basins using *aggregate rainfall-runoff conceptual models*. So far, the platforms includes three different well known models:

1. *GR4J*
2. *HBV*
3. *HYMOD*

These models run at *daily time steps* and are able to estimate daily runoff at the basin outlet.

**thym** is programmed in `C` language, and the pre- and postprocessing of model information is performed in `python`.

## Code structure
---

The main directory `/thym` is composed by the following directories and files:

1. `/bin`: Has the `makefile` and the model executables after compilation.
2. `/inc`: Has all the  headers `*.h` files.
3. `/src`: Has all the source code `*.c` files.
4. `/test_[model]`, : Application of a model (e.g. `GR4J`, `HBV` or `HYMOD`) to test case. For instance, `test_gr4j` contain the input files needed to apply the `GR4J` model.  This directory contains the input files:
5. `/tools`: Has `python` scripts for model result postprocessing. The `plot_in_out.py` script plots simulated and observed time series and the variables in `meteo.in` input file. 

### `ctrl.in` file
This is the control or steering file for **thym**. This file has the following structure:

|ROW|MEANING|
|--------|---------------------------------------------------------|
| row 1  | Watershed name and hydrometric station                  |
| row 2  | Outlet latitude [dec]                                   |
| row 3  | Outlet longitude [dec]                                  |
| row 4  | Outlet altitude in [m.a.s.l]                            |
| row 5  | Watershed area in [km^2]                                |
| row 6  | Starting time (yyyy mm dd hr[0 23] min[0 59] sec[0 59]) |
| row 7  | End time (yyyy mm dd hr[0 23] min[0 59] sec[0 59])      |
| row 8  | Time step (s)                                           |
| row 9  |                                                         |
| row 10 | Variables to write out (NOT USED)                       |
| row 11 | Interval for writing output (s) (NOT USED)              |
| row 12 |                                                         |
| row 13 | PET method ['obs', 'hamon0', 'hamon1', 'priestley']     |
| row 14 |                                                         |
| row 15 | Model's name ['gr4j' 'hbv' 'hymod']                     |
| row 16 | Model's parameters                                      |
|--------|---------------------------------------------------------|

Note that:
- In *row 13*: The user can chose the method to estimated the daily *potential evapotranspiration* among the following options: *obs* [observed precomputed values introduced through the `meteo.in` file], *hamon0* [Hamon method version 0], *hamon1* [Hamon method version 1 (more standard)] and *priestly* [Priestley method].  

- In *row 16*: This row contains the parameters corresponding to the model in *row 15*. According to the model, the parameters are as follow:

#### GR4J

| col 1  | x1     | Maximum capacity of the production store [mm]                      |
| col 2  | x2     | Groundwater recharge coefficient                                   |
| col 3  | x3     | One-day ahead maximun capacity of the routing store [mm]           |
| col 4  | x4     | Time base of unit hydrograph UH1                                   |
| col 5  | trs    | Air temperature above which the precipitation is all rainfall [oC] |
| col 6  | tmlt   | The base temperature above which snow melt is allowed [oC]         |
| col 7  | sno50  | The depth of snow when the basin is 50% covered by snow [mm]       |
| col 8  | sno100 | The shreshold depth of snow, above which there is 100% cover [mm]  |
| col 9  | ls     | Snow temperature lag factor                                        |
| col 10 | bmlt6  | Melt factor on June 21 [mm H2O/day-oC]                             |
| col 11 | bmlt12 | Melt factor on December 21 [mm H2O/day-oC]                         |
 
Note that the parameters *x1*, *x2*, *x3* and *x4* are often subjected to calibration. The others parameters belong to the *snow model* and are not relevant in tropical basins modelling.

#### HBV

|--------|--------|
| col 1  | k2     |
| col 2  | k1     |
| col 3  | k0     |
| col 4  | degd   |
| col 5  | degw   |
| col 6  | ttlim  | 
| col 7  | perc   | 
| col 8  | beta   |
| col 9  | lp     |
| col 10 | fcap   |
| col 11 | hl1    |
| col 12 | maxbas |


#### HYMOD

| col 1  | ks    | Slowflow routing tank's rate parameter - Range [0, 1]
| col 2  | kq    | Quickflow routing tanks' rate parameter - Range [0, 1]
| col 3  | ddf   | Degree day factor - Range [ 0, 2]
| col 4  | tb    | Base temperature to calculate melt - Range [-5, 5]
| col 5  | tth   | Temperature threshold - Range [-5, 5] 
| col 6  | alpha | Quick/slow split parameter - Range [0, 1]
| col 7  | b     | Scaled distribution function shape parameter - Range [0, 2]
| col 8  | huz   | Maximum height of soil moisture accounting tank - Range [0, Inf]


### `meteo.in` file 
This file contains the *meterological data* recorded within the catchment. This file has the following columns:

| datetime            | tave_oC | tmin_oC | tmax_oC | precip_mm | evapt_mm | runoff_mm |
|---------------------|---------|---------|---------|-----------|----------|-----------|
| 1981-01-01 00:00:00 | -3.5    | 0.0     | 0.0     | 7.21      | 0.0      | 1.0502    |
| ...                 | ...     | ...     | ...     | ...       | ...      | ...       |

where:

- *datetime*: Date time following the format: *YYYY-MM-DD HH:MM:SS*
- *tave_oC*: Daily average air temperature [$^oC$].
- *tmin_oC*: Daily minimum air temperature [$^oC$].
- *tmax_oC*: Daily maximum air temperature [$^oC$].
- *precip_mm*: Daily precipitation [$mm$].
- *evapt_mm*: Daily potential evapotranspiration [$mm$]. This can be either calculated within the model (see `ctrl.in` file) or can be introduced in this file. If the *evapotranspiration* is calculated within the model, this column can be equal to 0. 
- *runoff_mm*: Daily runoff at the outlet of the basin [$mm$].

and the output file:


### `results.out` file 
This file is composed by columns that records the values of *state variables* and *flux variables* for each simulated day. The variables (columns) are different for each model. Note that the most relevant variable, the *total catchment runoff* is called *q_mm* for any model and is given in *mm*. 

## Compilation, execution and plotting
---

### Compilation

1. Go to '/bin'. 
2. Type `make clean` this will remove `*.o` files and the executable (optional).
3. Within `makefile` change the variable `MODEL`. So if:
    - `MODEL=1` compilation is performed for *GR4J* model
    - `MODEL=2` compilation is performed for *HBV* model
    - `MODEL=3` compilation is performed for *HYMOD* model
4. Type `make`. This will execute the `makefile` and create `thym_[model]` (e.g `thym_gr4j`) (*model executable*) and a *symboly link* to the executable within the main dir `/thym`.

### Execution
1. Go to the root dir `/thym`.
2. Type `./thym_[model] testcase`, so for instance `thym_gr4j test_gr4j`. This will run *GR4J* model for the *test_gr4j* study case. If the model execution is succesfull, the model will log results in the file `results.out` saved within *testcase* directory (e.g. `/test_gr4j`).


### Ploting
1. Go to `/tools`
2. Type `./plot_in_out.py ../testcase`, so for instance `./plot_in_out.py ../test_gr4j`. `plot_in_out.py` will plot time series of *simulated* (*q_mm* variable in the `results.out` output file) and *observed* (*runoff_mm* variable in the `meteo.in` input file) runoff in *mm*. The script will plot also all the variables in `meteo.in` file. 

Note that *compilation, execution and plotting* can be performed all at once using the `run.sh` in `/thym` root dir typing `./run.sh testcase` (e.g. `./run.sh test_gr4j`). Every step is performed as long as: `COMP=true EXEC=true  PLOT=true`.
