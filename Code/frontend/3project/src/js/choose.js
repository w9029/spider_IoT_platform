
export default {
  produce:"",
  setProduce(c_name) {
  this.produce=c_name;
},
 getProduce(c_name) {
  return this.produce
},
  delProduce(c_name){
    this.produce='';
  },
}
