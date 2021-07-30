// global data struct of uchar/ int 8 3X3 grid
var gameBoard = [ [0, 0 ,0],
                  [0, 0 ,0],
                  [0, 0 ,0],
                ];
var pPadding = 0.1;
// list of available positions left

// method to make player move
  // get number input
  // if valid enter move
    // checkValid
    // update global
    // draw
    // checkwin
    // computer move
  // else do nothinging and wait for input

// method to make computer move
  // pick random number, from list, list is shortend when update global is called
  // place peice
  // draw
  // checkwin
  // player turn

// check valid move methods
  // if move in list of availbe positions

// check win method
  // row by row if start of row is the same as all win start of row
  // col by col if start of col is same as rest of call win
  // diag 1 if 1,1 2,2 3,3 are same then w
  // diag 2 if 1,3 2,2 3,1 are same then w
  // if win return win and draw on canvas line accross

//draw
  // make a box in canvas
    // draw line vertical 1/3 and 2/3 of the way
    // draw line horizontal 1/3 and 1/3 of the way
    // for all center positions defined by 1/6,1/6 3/6,1/6 5/6,1/6 ect
        // draw an x for 1 nothing for 0 and o for -1

function DrawBox(w,h,ctx,c){
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
