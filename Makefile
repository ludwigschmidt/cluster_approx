# Ludwig Schmidt (ludwigschmidt2@gmail.com) 2014
#
# This makefile is based on http://make.paulandlesley.org/autodep.html .

CXX = g++
MEX = mex
CXXFLAGS = -Wall -Wextra -O3 -fPIC
MEXCXXFLAGS = -Wall -Wextra -O3
GTESTDIR = /usr/src/gtest

SRCDIR = src
DEPDIR = .deps
OBJDIR = obj

SRCS = cluster_grid.cc pcst_fast.cc cluster_grid_pcst_mex_wrapper.cc

.PHONY: clean archive

clean:
	rm -rf $(OBJDIR)
	rm -rf $(DEPDIR)
	rm -f cluster_grid_pcst.mex*


mexfiles: cluster_grid_pcst_mexfile


CLUSTER_GRID_OBJS = cluster_grid.o pcst_fast.o

CLUSTER_GRID_MEXFILE_SRC = cluster_grid_pcst_mex_wrapper.cc cluster_grid.cc pcst_fast.cc
CLUSTER_GRID_MEXFILE_SRC_DEPS = $(CLUSTER_GRID_MEXFILE_SRC) mex_helper.h cluster_grid.h

cluster_grid_pcst_mexfile: $(CLUSTER_GRID_MEXFILE_SRC_DEPS:%=$(SRCDIR)/%)
	$(MEX) -v CXXFLAGS="\$$CXXFLAGS $(MEXCXXFLAGS)" -output cluster_grid_pcst $(CLUSTER_GRID_MEXFILE_SRC:%=$(SRCDIR)/%)


$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	# Create the directory the current target lives in.
	@mkdir -p $(@D)
	# Compile and generate a dependency file.
	# See http://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html .
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<
	# Move dependency file to dependency file directory.
	# Create the dependency file directory if necessary.
	@mkdir -p $(DEPDIR)
	@mv $(OBJDIR)/$*.d $(DEPDIR)/$*.d

# Include the generated dependency files.
# The command replaces each file name in SRCS with its dependency file.
# See http://www.gnu.org/software/make/manual/html_node/Substitution-Refs.html#Substitution-Refs for the GNU make details.
-include $(SRCS:%.cc=$(DEPDIR)/%.d)
