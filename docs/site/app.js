const views=[...document.querySelectorAll('.view')];
const controls=[...document.querySelectorAll('[data-view]')];
function showView(name){views.forEach(v=>v.classList.toggle('active-view',v.id===name));controls.forEach(c=>c.classList.toggle('active',c.dataset.view===name));window.scrollTo({top:0,behavior:'smooth'});}
controls.forEach(c=>c.addEventListener('click',()=>showView(c.dataset.view)));
const send=document.getElementById('send');
const endpoint=document.getElementById('endpoint');
const body=document.getElementById('responseBody');
const status=document.getElementById('status');
if(send)send.addEventListener('click',()=>{status.textContent='200 OK';body.textContent=JSON.stringify({success:true,method:document.getElementById('method').value,endpoint:endpoint.value,service:'aetherworld',engine:'CircleMUD',status:'online',version:'3.5.0',note:'Static GitHub Pages console; configure a server endpoint for live API requests.'},null,2)});
