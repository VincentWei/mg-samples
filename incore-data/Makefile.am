noinst_PROGRAMS=incore-data

incore_data_SOURCES=png_pics.inc unifont_160_50.c incore-data.c

BUILT_SOURCES = png_pics.inc unifont_160_50.c

EXTRA_DIST=res

# upf2c is a tool included in mg-tools:
#
#   https://github.com/VincentWei/mg-tools
#
# Please make sure to make and install the tools to your Linux system first.
unifont_160_50.c:
	upf2c res/unifont_160_50.upf unifont_160_50

# bin2c is a tool included in mg-tools:
#
#   https://github.com/VincentWei/mg-tools
#
# Please make sure to make and install the tools to your Linux system first.
png_pics.inc:
	bin2c -o png_pics.inc res/*.png

clean-local: clean-local-check
.PHONY: clean-local-check
clean-local-check:
	rm -f unifont_160_50.c png_pics.inc
