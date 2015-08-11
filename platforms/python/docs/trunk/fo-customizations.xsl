<?xml version='1.0'?> 
<xsl:stylesheet  
       xmlns:xsl="http://www.w3.org/1999/XSL/Transform"  version="1.0"> 
  <xsl:import href="/usr/share/xml/docbook/stylesheet/nwalsh/fo/docbook.xsl"/> 
  <xsl:include href="common-customizations.xsl" />
  <xsl:param name="fop1.extensions">1</xsl:param>
  <xsl:param name="paper.type">A4</xsl:param>
  <xsl:attribute-set name="monospace.verbatim.properties">
    <xsl:attribute name="wrap-option">wrap</xsl:attribute>
    <xsl:attribute name="hyphenation-character">\</xsl:attribute>
    <xsl:attribute name="font-size">
      <xsl:choose>
        <xsl:when test="processing-instruction('db-font-size')"><xsl:value-of
           select="processing-instruction('db-font-size')"/></xsl:when>
        <xsl:otherwise>inherit</xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
  </xsl:attribute-set>
</xsl:stylesheet> 
