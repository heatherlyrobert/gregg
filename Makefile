#================================[[ beg-code ]]================================#
include /usr/local/sbin/make_prepare



#===(user defined)=========================================#
# basename of executable, header, and all code files
NAME_BASE  = gregg
# additional standard and outside libraries
LIB_STD    = $(LIB_OGLSTD)
# all heatherly libraries used, debug versions whenever available
LIB_MINE   = $(LIB_OPENGL) -lyPARSE_debug -lySORT_debug -lyEXEC_debug -lyJOBS_debug
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
