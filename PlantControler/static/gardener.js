
function DrawBox(w,h){
  let c = document.getElementById("myCanvas");
  let ctx = c.getContext("2d");
  let canvasWidth = c.width;
  let canvasHeight = c.height;
  let beginPosx = (canvasWidth/2) - (w/2);
  let endPosx =  (canvasWidth/2) + (w/2);
  let beginPosy = (canvasHeight/2) - (h/2);
  let endPosy =  (canvasHeight/2) + (h/2);
  ctx.beginPath();
  ctx.rect(beginPosx, beginPosy, endPosx, endPosy);
  ctx.stroke();
}
function DrawGame(){
  let c = document.getElementById("myCanvas");
  let ctx = c.getContext("2d");
  //DrawBox(20,20,ctx,c);
  ctx.moveTo(0, 0);
  ctx.lineTo(500, 500);
  ctx.stroke();
  fillstring = "[lively]";
  ctx.fillText(fillstring, 0, c.height-100);
}
