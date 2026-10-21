document.addEventListener('click',e=>{const b=e.target.closest('[data-action="heal"]');if(!b)return;S.hp=S.maxHp;S.energy=S.maxEnergy;note('You rest at the inn. HP and energy restored.');render()});
