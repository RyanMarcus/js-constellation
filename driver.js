// { begin copyright } 
// Copyright Ryan Marcus 2016
// 
// This file is part of js-constellation.
// 
// js-constellation is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// js-constellation is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with js-constellation.  If not, see <http://www.gnu.org/licenses/>.
// 
// { end copyright }


"use strict";

document.addEventListener("DOMContentLoaded", function (event) {
    let m = Module();
    let createSky = m.cwrap("createSky", "number", ["number", "number",
                                                         "number", "number",
                                                         "number"]);
    let progress = m.cwrap("progress", "number", ["number"]);
    let drawLines = m.cwrap("drawLines", null, ["number", "number", "number"]);
    let getStarX = m.cwrap("getStarX", "number", ["number", "number"]);
    let getStarY = m.cwrap("getStarY", "number", ["number", "number"]);
    let readStarX = m.cwrap("readStarX", "number", ["number"]);
    let readStarY = m.cwrap("readStarY", "number", ["number"]);
    let getStarAt = m.cwrap("getStarAt", "number", ["number", "number"]);



    let canvas = document.getElementById("myCanvas");
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;
    
    let ctx = canvas.getContext('2d');

    ctx.strokeStyle = 'rgba(243, 255, 226, 1)';
    ctx.fillStyle = 'rgba(243, 255, 226, 1)';
    ctx.lineWidth = 0.4;
    
    let numStars = 300;

    let lastTS = null;

    let drawBufferFP = m.Runtime.addFunction(function(stars, numStars) {
        for (let i = 0; i < numStars; i += 2) {
            let starPtr1 = getStarAt(stars, i);
            let starPtr2 = getStarAt(stars, i+1);
            ctx.beginPath();
            ctx.moveTo(readStarX(starPtr1), readStarY(starPtr1));
            ctx.lineTo(readStarX(starPtr2), readStarY(starPtr2));
            ctx.stroke();
            ctx.closePath();
        }
    });



    //int numStars, unsigned int randSeed, float maxVelocity, float xBound, float yBound
    let sky = createSky(numStars, 5, 0.5, canvas.width, canvas.height);
    function loop(ts) {
        window.requestAnimationFrame(loop);
        
        if (lastTS && ts - lastTS < 50)
            return;

        ctx.clearRect(0, 0, canvas.width, canvas.height);

        lastTS = ts;

        sky = progress(sky);

        for (let i = 0; i < numStars; i++) {
            ctx.beginPath();
            ctx.arc(getStarX(sky, i), getStarY(sky, i), 0.15, 0, Math.PI*2, true);
            ctx.fill();
            ctx.closePath();
        }

        drawLines(sky, 45, drawBufferFP);
        

    }


    window.requestAnimationFrame(loop);
});
