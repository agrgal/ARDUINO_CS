<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>ArduinoBasics WebServer Data Viewer</title>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.3/jquery.min.js"></script>
</head>
<body>
<style>
  .table{
    width: 100%;
    background: #eeeeee;
    line-height: 32px;
    border: 1px solid black;
  }
  .bars { 
    float: left;
    height: 30px;
    line-height: 30px;
    border: 1px solid black;
    padding-left: 10px;
    padding-right: 10px;
    background: #FFFF33;
    color: #000000;
    }
  .row{
    width: 100%;
    clear: both;
  }
</style>
<div align="center" style="background: #eeeeee; border: 1px solid black;">
  <h1>ArduinoBasics Data Viewer</h1>
</div>
<br>
<div id="IP Address" style="width: 220px; float:left" align="center">IP Address</div><div id="Port Num" style="width: 200px; float:left;" align="center">Port</div><br>
<select id="Addr0"></select> .
<select id="Addr1"></select> .
<select id="Addr2"></select> .
<select id="Addr3"></select> :
<input id="port" value='8081' /><br>
<button type="button" id="startData" style="height: 50px; width: 228px">Click here to start getting data</button>&nbsp;&nbsp;
<button type="button" id="stopData" style="height: 50px; width: 172px">Click here to stop </button><br>&nbsp;
<br>
<div class="table">
  <div class="row"><div class="bars">Tiempo: </div><div class="bars" id="tiempo"></div><div class="bars" id="ti">0</div></div><br>
  <div class="row"><div class="bars">Temperatura: </div><div class="bars" id="temperatura"></div><div class="bars" id="te">0</div></div><br>  
</div>
<br> &nbsp;
<div id="FailureStatus"><H2>Status:</H2></div>
<br> &nbsp;
<div id="statusDiv"><i>This viewer is <b>not compatible</b> with Internet Explorer, and requires JavaScript.</i></div>

<script>
<!--This is the jQuery script which will connect to the Arduino -->
  var timeOut; //This variable is responsible for the frequency of data acquisition 
  
  $(document).ready(function(){
      //Populate the options for the IP address drop-down boxes.
      for(j=0; j<256; j++){
        for(i=0; i<4; i++){
          $('#Addr'+i)
            .append($("<option></option>")
            .attr("value",j)
            .text(j));
      } 
    }   
    
    //Set the default IP address in the drop down boxes.
    $('#Addr0').val('192');
    $('#Addr1').val('168');
    $('#Addr2').val('1');
    $('#Addr3').val('35');
    
  });
  
  
  //When the start button is clicked - get the data from the Arduino
  $("#startData").click(function(){
    $(document).ready(function(){
         getMyData(); //get data once the document is ready
    });
  });  
  
  
  //Stop collecting data when the stop button is clicked.
  $("#stopData").click(function(){
      clearTimeout(timeOut);        //This clears any future requests for data (until the Start button is pressed again)
  });
  
  function getMyData(){ 
    //get the IP address from the drop-down boxes
    var x0 = $('#Addr0 option:selected').text();
    var x1 = $('#Addr1 option:selected').text();
    var x2 = $('#Addr2 option:selected').text();
    var x3 = $('#Addr3 option:selected').text();
    
    //get the port number from the text box
    var x4 = $('#port').val(); 
     
    //Construct the URL of the Arduino Server we plan to connect to
    var myUrl = 'http://' + x0 + "." + x1 + "." + x2 + "." + x3 + ":" + x4 + "/";
    
    var myData = $.ajax({
          url: myUrl,              // eg.  http://10.1.1.99:8081/
          data: { tag: 'GetDataFromArduino'},
          dataType : "json",      //We will be requesting data in JSON format
          timeout : 10000,        //this will cancel the request if it has not received a reply within 10 seconds.
          success: function(data){
              console.log('Success - you are a legend');
              $("#FailureStatus").html("<H2>Status: OK</H2>");  //clear any failure messages.
              $.each(data, function(index, element) { 
                /*
                if(element.value<100){
                    console.log('Low'); 
                    $('#Analog' + element.key).css({'background-color':'#FF1128'}); 
                } else {
                    $('#Analog' + element.key).css({'background-color':'#22FF22'});
                }
              
                $('#A'+element.key).html("<span>" + element.value + "</span>");
                $('#Analog' + element.key).animate({width: ((element.value/1023)*80)+"%"}); */
                
                // alert(element.tiempo+" "+element.temperatura);
                $('#ti').html("<span>" + element.tiempo + "</span>");
                $('#te').html("<span>" + element.temperatura + "</span>");
              });
        }});
        
      myData.error(function(xhr, status, errorThrown){
         
              $("#FailureStatus").html("<span><H2>Status:FAILED to get DATA !! </H2>"+myUrl+"</SPAN>"); 
       
			  console.log('Failure'); 
			  console.log("Error: " + errorThrown); 
			  console.log("Status: " + status);
			  console.dir(xhr);
       });
    
    timeOut = setTimeout('getMyData()', 1000); //this will request data again in 1 second.
  }
</script>
</body>
</html>
