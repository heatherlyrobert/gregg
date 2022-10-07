#================================[[ beg-code ]]================================#



#===(user defined)=========================================#
# basename of executable, header, and all code files
NAME_BASE  = gregg
# additional standard and outside libraries
LIB_STD    = -lm -lX11 -lGL -lGLU
# all heatherly libraries used, debug versions whenever available
LIB_FOUND  = -lyMODE_debug  -lyKEYS_debug  -lyFILE_debug  -lyVIEW_debug
LIB_MAJOR  = -lyMAP_debug   -lyCMD_debug   -lyMACRO_debug -lySRC_debug   -lyMARK_debug
LIB_OPENGL = -lyVIOPENGL_debug -lyX11_debug  -lyFONT_debug  -lyCOLOR_debug -lyGLTEX_debug
LIB_OTHER  = -lyVAR_debug   -lySTR_debug   -lyPARSE_debug
LIB_MINE   = $(LIB_FOUND)   $(LIB_MAJOR)   $(LIB_OPENGL)   $(LIB_OTHER)
# directory for production code, no trailing slash
INST_DIR   = /usr/local/bin



#===(optional)=============================================#
# extra include directories required
INC_MINE   = 
# utilities generated, separate from main program
NAME_UTIL  = 
# libraries only for the utilities
LIB_UTIL   = 



#===(post-processing)======================================#
# create a rule for...
#install_prep       :
#remove_prep        :



#===(master template)======================================#
include /usr/local/sbin/make_program



#===(post-processing)======================================#
# create a rule for...
#install_post       :

#remove_post        :



#================================[[ end-code ]]================================#
