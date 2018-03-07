CGC := psp2cgc
CGCFLAGS := -Wperf -fastprecision -O3

SHSRC := $(wildcard *.cg)
SHBIN := $(SHSRC:.cg=.gxp)
OUTDIR := ../build/shaders

all: $(SHBIN)

%_f.gxp: %_f.cg
	$(CGC) -profile sce_fp_psp2 $< $(CGCFLAGS) -o $(OUTDIR)/$@

%_v.gxp: %_v.cg
	$(CGC) -profile sce_vp_psp2 $< $(CGCFLAGS) -o $(OUTDIR)/$@

clean:
	rm -rf $(OUTDIR)/*.gxp
