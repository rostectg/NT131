#include <linux/init.h> /* Needed for the macros */
#include <linux/kernel.h> 
#include <linux/module.h> /* Needed by all modules */

#include <linux/netfilter.h>
#include <linux/ip.h>
#include <linux/netfilter_ipv4.h>

static unsigned int hook_block_icmp(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
    struct iphdr *iph;   // IP header
    if(!skb)
        return NF_ACCEPT;
    iph = ip_hdr(skb); // IP headers

    if (iph->protocol == IPPROTO_ICMP) {
        pr_info("DROP_ICMP: Dropped ICMP packet\n");
        return NF_DROP;
    }
    else {
        return NF_ACCEPT;
    }
}


static struct nf_hook_ops *ops = NULL;

static int __init main_init(void) {
    ops = (struct nf_hook_ops*)kcalloc(1,  sizeof(struct nf_hook_ops), GFP_KERNEL);
    if (ops != NULL) {
        ops->hook = (nf_hookfn*)hook_block_icmp;
        ops->hooknum = NF_INET_PRE_ROUTING;
        ops->pf = NFPROTO_IPV4;
        ops->priority = NF_IP_PRI_FIRST;

        nf_register_net_hook(&init_net, ops);
        pr_info("DROP_ICMP: Hook installed\n");
    }
    return 0;
}

static void __exit main_exit(void) {
    if(ops != NULL) {
        nf_unregister_net_hook(&init_net, ops);
        kfree(ops);
    }
    pr_info("DROP_ICMP: Exit\n");
}

module_init(main_init);
module_exit(main_exit);
MODULE_LICENSE("GPL");