import{d as y,r as h,a as x,t as C,c as r,w as b,b as o,e as p,v as S,f as u,g as $,h as w,u as d,F as B,i as L,o as c,j as v,n as T,k as E,E as N,_ as V}from"./index-DkkuTocZ.js";import{S as z,_ as D}from"./stage-Bx77invL.js";const F={class:"container"},M={class:"welcome"},j={class:"score"},q={class:"stage"},H=["onClick"],I=y({__name:"pc",setup(O){const l=h(!0);let n=x(new z(8,8,50));const{data:_,score:a}=C(n),g=()=>{l.value=!1,n.gameLoop(!0)},f=t=>{n.click(t)},m=()=>{N.alert(k(a.value),"雪糕消消大作战",{confirmButtonText:"确定",callback:t=>{l.value=!0},dangerouslyUseHTMLString:!0})},k=t=>t>17?`
    当前成绩：${a.value}分
    <div class="result">
      <p>恭喜你获得了</p>
      <p>雪糕大王称号</p>
      <p>flag在根目录下/tgflagggg中</p>
    </div>
    `:`
    当前成绩：${a.value}分
    <div class="result">
      <p>菜！！！</p>
    </div>
    `;return(t,s)=>{const i=$("el-button");return c(),r("div",F,[b(o("div",M,[s[1]||(s[1]=o("img",{class:"title-img",src:D},null,-1)),p(i,{class:"change-btn",type:"primary",onClick:g},{default:u(()=>s[0]||(s[0]=[v("开始游戏")])),_:1})],512),[[S,l.value]]),o("div",j,w(d(a))+"分",1),o("div",q,[(c(!0),r(B,null,L(d(_),e=>(c(),r("div",{style:E({left:`${e.positionLeft}px`,top:`${e.positionTop}px`}),key:e.key,class:T(["square",`type${e.type}`,`scale${e.scale}`,{active:e.active}]),onClick:R=>f(e)},null,14,H))),128))]),p(i,{class:"change-btn",type:"primary",onClick:m},{default:u(()=>s[2]||(s[2]=[v("结束游戏")])),_:1})])}}}),G=V(I,[["__scopeId","data-v-9b2861e2"]]);export{G as default};
