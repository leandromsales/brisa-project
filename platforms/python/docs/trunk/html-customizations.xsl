<?xml version='1.0'?> 
<xsl:stylesheet  
       xmlns:xsl="http://www.w3.org/1999/XSL/Transform"  version="1.0"> 
  <xsl:import href="/usr/share/xml/docbook/stylesheet/nwalsh/html/onechunk.xsl"/> 
  <xsl:include href="common-customizations.xsl" />
  <xsl:param name="html.stylesheet">css/docbook2.css</xsl:param>
  <xsl:param name="chunker.output.indent">yes</xsl:param>
  <xsl:param name="use.id.as.filename">1</xsl:param>
  <xsl:param name="chunker.output.encoding">UTF-8</xsl:param>
  <xsl:param name="bibliography.numbered">1</xsl:param>
  <xsl:param name="ignore.image.scaling">1</xsl:param>
</xsl:stylesheet> 
