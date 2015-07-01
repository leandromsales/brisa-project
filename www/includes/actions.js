// JavaScript Document

function ajaxHTML(id,url){

//Obtém o objeto HTML
objetoHTML=document.getElementById(id);

//Exibe "Carregando..."
objetoHTML.innerHTML = "<img src='images/loading.gif'/>"
try{
xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");//mudei a posição
}catch(ee){
try{
xmlhttp = new ActiveXObject("Msxml2.XMLHTTP");
}catch(e){
try{
xmlhttp = new XMLHttpRequest();//mudei a posição
}catch(E){
xmlhttp = false;
}
}
}

xmlhttp.open("GET",url);
xmlhttp.onreadystatechange=function() {
if (xmlhttp.readyState==4){

retorno=unescape(xmlhttp.responseText.replace(/\|/g," "));
objetoHTML.innerHTML=retorno;
}
}

xmlhttp.send(null);
};

mostrarRelogio=function(){
horaAtual = new Date()
hora = horaAtual.getHours()
minuto = horaAtual.getMinutes()
segundo = horaAtual.getSeconds()

horaImprimivel = hora + " : " + minuto + " : " + segundo

document.getElementById('relogio').value=""+ horaImprimivel;

setTimeout("mostrarRelogio()",1000);
};

function exibir(id){
	var element = document.getElementById(id);
	element.style.display = 'block';
};