import{d as h,r as b,a as x,t as C,c as r,w,b as o,e as p,v as S,f as u,g as $,h as B,u as d,F as E,i as L,j as g,n as T,k as N,o as c,E as V,_ as z}from"./index-DkkuTocZ.js";import{S as D,_ as F}from"./stage-Bx77invL.js";const M={class:"container"},j={class:"welcome"},q={class:"score"},H={class:"stage"},I=["onClick"],O=h({__name:"mobile",setup(R){const l=b(!0);let m=document.documentElement.clientWidth;const n=x(new D(7,7,(m-20)/7)),{data:v,score:a}=C(n),_=()=>{l.value=!1,n.gameLoop(!0)},f=t=>{n.click(t)},k=()=>{V.alert(y(a.value),"雪糕消消大作战",{confirmButtonText:"确定",callback:t=>{l.value=!0},dangerouslyUseHTMLString:!0})},y=t=>t>17?`
    当前成绩：${a.value}分
    <div class="result">
      <img src="@/assets/images/result.png" />
      <p>恭喜你获得了</p>
      <p>雪糕大王称号</p>
      <p>flag在根目录下/tgflagggg中</p>
     </div>
    `:`
    当前成绩：${a.value}分
    <div class="result">
      <img src="@/assets/images/result.png" />
      <p>菜！！！</p>
    </div>
    `;return(t,s)=>{const i=$("el-button");return c(),r("div",M,[w(o("div",j,[s[1]||(s[1]=o("img",{class:"title-img",src:F},null,-1)),p(i,{class:"change-btn",type:"primary",onClick:_},{default:u(()=>s[0]||(s[0]=[g("开始游戏")])),_:1})],512),[[S,l.value]]),o("div",q,B(d(a))+"分",1),o("div",H,[(c(!0),r(E,null,L(d(v),e=>(c(),r("div",{style:N({left:`${e.positionLeft}px`,top:`${e.positionTop}px`}),key:e.key,class:T(["square",`type${e.type}`,`scale${e.scale}`,{active:e.active}]),onClick:U=>f(e)},null,14,I))),128))]),p(i,{class:"change-btn",type:"primary",onClick:k},{default:u(()=>s[2]||(s[2]=[g("结束游戏")])),_:1})])}}}),G=z(O,[["__scopeId","data-v-c9ae4bd8"]]);export{G as default};
