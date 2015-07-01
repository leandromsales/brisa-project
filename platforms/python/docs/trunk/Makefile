HTMLSTYLE=html-customizations.xsl
FOSTYLE=fo-customizations.xsl

%.html: %.xml 
	xsltproc  						\
	   --output $@ 						\
	   $(HTMLSTYLE) $<
	mv index.html $@
	
html:  documentation-enduser.html documentation-developer.html


%.fo: %.xml 
	xsltproc  						\
	   --output $@ 						\
	   $(FOSTYLE) $<
	
fo:  documentation-enduser.fo documentation-developer.fo


%.pdf: %.fo 
	fop-0.95/fop  						\
	   -pdf $@ 						\
	   -fo $<
	
pdf:  documentation-enduser.pdf documentation-developer.pdf



clean:
	-rm -f *.html
	-rm -f *.fo
	-rm -f *.pdf
